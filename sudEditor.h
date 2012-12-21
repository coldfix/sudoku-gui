#pragma once

#include "mSudoku.h"
#include "sudSolve.h"

class sudSolve;


class sudEditor
{
public:
    sudEditor(int nbx, int nby);
    ~sudEditor();

    const sudSolve& GetSolver() const               { return m_solver; }

    mSudoku GetShown() const                        { return m_shown; }
    const Sudoku& GetRiddle() const                 { return m_riddle; }
    const Sudoku& GetUser() const                   { return m_user; }
    const Sudoku& GetSolution() const               { return m_solution; }
    const sudSize& GetSize() const                  { return GetRiddle(); }

    int GetRiddle(int x, int y) const               { return m_riddle.get(x, y); }
    int GetUser(int x, int y) const                 { return m_user.get(x, y); }
    int GetSolution(int x, int y) const             { return m_solution.get(x, y); }

    int GetCell(int x, int y) const                 { return m_shown.get(pt(x, y)); }
    int GetCellFlags(int x, int y) const            { return m_shown.getflags(pt(x, y)); }
    int GetCellMode(int x, int y) const             { return m_shown.getmode(pt(x, y)); }
    bool IsReadOnly(int x, int y) const             { return !IsEditMode() && GetRiddle(x, y); }

    bool IsFalseInput(int x, int y) const;
    bool IsFalseInput(int x, int y, int val) const;

    int GetUnsolved() const;


    bool GetShowSolution() const                    { return m_state & state_showsolution; }
    bool IsEditMode() const                         { return m_state & state_initmode; }


    virtual bool SetEditMode(bool on);
    virtual void SetShowSolution(bool on);

    virtual bool SetSize(int nbx,int nby);
    virtual bool SetRiddle(const Sudoku&sriddl);
    virtual bool SetShown(const mSudoku& shown);

    virtual bool SetCell(int x, int y, int val);

    void ClearSudoku();



    int GetRefreshLevels() const                    { return m_refreshlev; }
    void SetRefreshLevels(int l)                    { m_refreshlev = l; }


/*  bool Generate(int difficulty);
    bool Refill(int difficulty);
    bool Mutate(int difficulty);*/

protected:
    int pt(int x, int y) const { return m_shown.pt(x, y); }

    virtual void UpdateCell(int x, int y);
    virtual void UpdateAll();

    virtual void Solve();
    virtual void Refresh();

    sudSolve m_solver;
    Sudoku m_riddle, m_user, m_solution;
    mSudoku m_shown;

    enum{
        state_initmode = 1, state_showsolution = 2
    };

    int m_refreshlev, m_state;
};
