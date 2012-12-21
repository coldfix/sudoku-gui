#include <wx/wxprec.h>

#include "sudEditor.h"
#include "sudSolve.h"



sudEditor::sudEditor(int nbx,int nby)
    : m_refreshlev(15),
    m_state(0)
{
    SetSize(nbx, nby);
}

sudEditor::~sudEditor()
{
}


void sudEditor::UpdateCell(int x, int y)
{
    int mode;
    if (GetUser(x, y))
    {
        if (GetShowSolution() && IsFalseInput(x, y))
            mode = mSudoku::mode_incorrect;
        else
            mode = mSudoku::mode_user;
    }

    else if (GetShowSolution()
         && !GetRiddle(x, y)
         && GetSolution(x, y))
    {
        mode = mSudoku::mode_solution;
        if (IsFalseInput(x, y))
            mode |= mSudoku::flag_invalid;
    }

    else
    {
        mode = mSudoku::mode_riddle;
        if (IsFalseInput(x, y))
            mode |= mSudoku::flag_invalid;
    }

    m_shown.setflags(pt(x, y), mode);


    int val;
    if (GetRiddle(x, y))
        val = GetRiddle(x, y);
    else if (GetShowSolution() && GetSolution(x,y))
        val = GetSolution(x, y);
    else
        val = GetUser(x, y);
    m_shown.set(pt(x, y), val);
}

void sudEditor::UpdateAll()
{
    int h = GetSize().high();
    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < h; y++)
            UpdateCell(x, y);
    }
}


void sudEditor::SetShowSolution(bool on)
{
    if (on == GetShowSolution())
        return;

    if (on)
        m_state |= state_showsolution;
    else
        m_state &= ~state_showsolution;

    UpdateAll();
    Refresh();
}


bool sudEditor::IsFalseInput(int x, int y, int val) const
{
    if (!GetSolver().ok() || !val)
        return false;

    if (!GetRiddle(x, y))
        return !m_solver.possible(x, y, val);

    if (val == m_riddle.get(x,y))
        return false;

    Sudoku sud = m_riddle;
    sud.set(x, y, val);
    sudSolve tmpsolve(m_riddle);
    tmpsolve.solve(GetRefreshLevels());
    return tmpsolve.error();
}


bool sudEditor::IsFalseInput(int x, int y) const
{
    if (GetUser(x, y))
        return !m_solver.possible(x, y, GetUser(x,y));
    if (GetRiddle(x,y))
        return !m_solver.possible(x, y, GetRiddle(x,y));
    return m_solver.error(x, y);
}


bool sudEditor::SetEditMode(bool on)
{
    if (on == IsEditMode())
        return true;

    if (on)
    {
        m_state |= state_initmode;
        UpdateAll();
        Refresh();
        return true;
    }
    else
    {
        if ( !GetSolver().ok()
            || !GetSolver().unique()
            || GetSolver().toohard())
            return false;

        m_state &= ~(state_initmode | state_showsolution);
        UpdateAll();
        Refresh();
        return true;
    }
}

bool sudEditor::SetRiddle(const Sudoku& riddle)
{
    SetSize(riddle.bx(), riddle.by());
    m_riddle = riddle;
    Solve();

    UpdateAll();
    Refresh();
    return true;
}

bool sudEditor::SetShown(const mSudoku& shown)
{
    SetSize(shown.bx(), shown.by());
    m_riddle = shown.filter(mSudoku::mode_riddle);
    m_user = shown.filter(mSudoku::mode_user);
    Solve();
    SetEditMode(false);
    UpdateAll();
    Refresh();
    return true;
}


bool sudEditor::SetSize(int nbx, int nby)
{
    if (!nbx||!nby)
        return false;

    m_riddle.setsize(nbx, nby);
    m_user.setsize(nbx, nby);
    m_solution.setsize(nbx, nby);

    m_shown = m_riddle;

    m_solver.setsize(nbx, nby);
    m_solver.solve(0);

    m_state = state_initmode;
    return true;
}


void sudEditor::ClearSudoku()
{
    SetSize(GetSize().bx(), GetSize().by());
    Refresh();
}


bool sudEditor::SetCell(int x, int y, int val)
{
    if (IsEditMode())
    {
        if (GetRiddle(x, y) != val && !IsFalseInput(x, y, val))
        {
            m_riddle.set(x, y, val);
            Solve();
            m_user.clear();
            UpdateAll();
        }
        else
            return false;
    }

    else
    {
        SetShowSolution(false);

        if (!IsReadOnly(x, y) && GetUser(x, y) != val)
        {
            m_user.set(x, y, val);
            UpdateCell(x, y);
        }
        else
            return false;
    }

    Refresh();
    return true;
}

int sudEditor::GetUnsolved() const
{
    return GetRiddle().getunsolved()
            + GetUser().getunsolved()
            - GetSize().size();
}





void sudEditor::Solve()
{
    m_solver.init(m_riddle);
    m_solver.solve(GetRefreshLevels());
    m_solution = m_solver.sudoku();
}

void sudEditor::Refresh()
{
}


