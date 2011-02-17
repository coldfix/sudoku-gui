#include <wx/wxprec.h>


#include "sudEditor.h"
#include "sudSolve.h"



/*
	sudEditor
*/

sudEditor::sudEditor(int nbx,int nby)
	: m_refreshlev(15),m_showsolution(0)
{
	SetSize(nbx,nby);
}

sudEditor::~sudEditor()
{
}


int sudEditor::GetCellMode(int x,int y) const{
	int mode;
	if(GetUser(x,y)){
		if( GetShowSolution() && IsFalseInput(x,y) )
			mode = cellmode_user_incorrect;
		else
			mode = cellmode_user_correct;
	}else if(GetShowSolution() && !GetRiddle(x,y) && GetSolution(x,y)){
		mode = cellmode_solution;
		if(IsFalseInput(x,y))
			mode |= cellmode_invalid;	/* only when isinitmode, because we do not enter usermode if riddle is not ok */
	}else if( GetRiddle(x,y) || IsInitMode() ){
		mode = cellmode_riddle;
		if(IsFalseInput(x,y))
			mode |= cellmode_invalid;	/* only when isinitmode, because we do not enter usermode if riddle is not ok */
	}else
		mode = cellmode_user_correct ;
	return mode; }

void sudEditor::SetShowSolution(bool on){
	m_showsolution = on; }

bool sudEditor::IsFalseInput(int x,int y,int val) const{
	if(! GetSolver().ok() || !val )
		return false;
	if( !GetRiddle(x,y) )
		return ! m_solver.possible(x,y,val);
	if( val==m_riddle.get(x,y) )
		return false;
	valtype ov = GetRiddle(x,y);
	Sudoku sud = m_riddle;
	sud.set(x,y,val);
	sudSolve tmpsolve;
	tmpsolve.init(m_riddle);
	tmpsolve.solve(GetRefreshLevels());
	return ! tmpsolve.ok();
}

bool sudEditor::IsFalseInput(int x,int y) const{
	if(GetUser(x,y))
		return !m_solver.possible(x,y,GetUser(x,y));
	if(GetRiddle(x,y))
		return !m_solver.possible(x,y,GetRiddle(x,y));
	return !m_solver.ok(x,y); }

bool sudEditor::EnterInitMode(bool on){
	if(on){
		m_initmode = true;
		m_user.clear();		// todo: merge the user input with the riddle sudoku if possible
		return true; }
	if( !GetSolver().ok() || !GetSolver().unique() || GetSolver().toohard() )
		return false;
	m_initmode = false;
	return true; }

bool sudEditor::SetRiddle(const Sudoku&rid){
	SetSize(rid.bx(),rid.by());
	m_riddle = rid;
	RefreshProcAll();
	return true; }


bool sudEditor::SetSize(int nbx, int nby){
	if(!nbx||!nby)
		return false;

		m_riddle.setsize(nbx,nby);
		m_user.setsize(nbx,nby);
		m_solution.setsize(nbx,nby);
		m_solver.setsize(nbx,nby);
		m_solver.solve(0);

	m_initmode = true;
	m_showsolution = false;
	return true; }


bool sudEditor::SetCell(int x, int y, int val){
	if(IsInitMode()){
		if(GetRiddle(x,y)!=val && !IsFalseInput(x,y,val)){	/* IsFalseInput...? */
			m_riddle.set(x,y,val);
			RefreshProcCell(x,y);
		}else{
			return false;								// do anything better?
		}
	}else{
		SetShowSolution(false);							// stop showing solution when user types a new value
		if(!IsReadOnly(x,y) && GetUser(x,y)!=val)
			m_user.set(x,y,val);
		else
			return false;
	}
	return true; }

int sudEditor::GetCell(int x, int y) const{
	return GetRiddle(x,y) ?GetRiddle(x,y) : (GetShowSolution()&&GetSolution(x,y) ?GetSolution(x,y):GetUser(x,y)); }

int sudEditor::GetUnsolved() const{
	return GetRiddle()->getunsolved() + GetUser()->getunsolved() - GetRiddle()->size(); }


void sudEditor::RefreshProcAll(){
	m_solver.init(m_riddle);
	m_solver.solve(GetRefreshLevels());
	m_solution = m_solver.sudoku();
}

void sudEditor::RefreshProcCell(int x,int y){

	// currently there is no fast algorithm for evaluating the change of only one cell
	RefreshProcAll();
}



