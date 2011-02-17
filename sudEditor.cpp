#include "sudEditor.h"
#include "sudSolve.h"



/*
	sudEditor
*/

sudEditor::sudEditor(int nbx,int nby)
	:m_riddle(nbx,nby),m_user(nbx,nby),m_solution(nbx,nby),
	m_refreshlev(4),m_showsolution(0)
{
	m_solver = new sudSolve( GetRiddle() );
	SetSize(nbx,nby);
}

sudEditor::~sudEditor()
{
	delete m_solver;
}

const sudSolveBase::MyValView& sudEditor::GetPossibilities(int x,int y) const{
	return GetSolver()->getpossibilities( GetSize()->acrd_xy(x,y) ); }

int sudEditor::GetCellMode(int x,int y) const{
	int mode;
	if(GetUser(x,y)){
		if( GetShowSolution() && IsFalseInput(x,y) )
			mode = cellmode_user_incorrect;
		else
			mode = cellmode_user_correct;
	}else if(GetShowSolution() && !GetRiddle(x,y) && GetSolution(x,y))
		mode = cellmode_solution;
	else if( GetRiddle(x,y) || IsInitMode() ){
		mode = cellmode_riddle;
		if(IsFalseInput(x,y))
			mode |= cellmode_invalid;	/* only when isinitmode, because we do not enter usermode if riddle is not ok */
	}else
		mode = cellmode_user_correct ;
	return mode; }

void sudEditor::SetShowSolution(bool on){
	m_showsolution = on; }

bool sudEditor::IsFalseInput(int x,int y,int val) const{
//	return m_riddle.possible(x,y,val);	// simple testing.. avoids a lot of processing
	if(! GetSolver()->isok() || !val )
		return false;
	if( !GetRiddle(x,y) )
		return GetPossibilities(x,y).find(val)==GetPossibilities(x,y).end();
	if( val==m_riddle.get(x,y) )
		return false;
	valtype ov = GetRiddle(x,y);
	sudSudoku sud = m_riddle;
	sud.set(x,y,val);
	sudSolve tmpsolve(&m_riddle);
	tmpsolve.test(GetRefreshLevels());
	return ! tmpsolve.isok();
}

bool sudEditor::IsFalseInput(int x,int y) const{
	if(GetUser(x,y))
		return GetPossibilities(x,y).find(GetUser(x,y))==GetPossibilities(x,y).end();
	if(GetRiddle(x,y))
		return GetPossibilities(x,y).find(GetRiddle(x,y))==GetPossibilities(x,y).end();
	return ! GetPossibilities(x,y).size(); }

bool sudEditor::EnterInitMode(bool on){
	if(on){
		m_initmode = true;
		m_user.clear();		// todo: merge the user input with the riddle sudoku if possible
		return true; }
	if( !GetSolver()->isok() || !GetSolver()->isunique() || GetSolver()->toohard() )
		return false;
	m_initmode = false;
	return true; }

bool sudEditor::SetRiddle(const sudSudoku&rid){
	SetSize(rid.b_x(),rid.b_y());
	m_riddle.init(rid);
	RefreshProcAll();
	return true; }


bool sudEditor::SetSize(int nbx, int nby){
	if(!nbx||!nby)
		return false;

	m_riddle.setsize(nbx,nby);
	m_user.setsize(nbx,nby);
	m_solution.setsize(nbx,nby);
	m_initmode = true;
	m_showsolution = false;

	m_solver->refresh(0);
	m_initmode = true;
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
	return GetRiddle()->getunsolved() + GetUser()->getunsolved() - GetRiddle()->getfieldnum(); }


void sudEditor::RefreshProcAll(){
	m_solver->refresh(GetRefreshLevels());
	m_solution.init( m_solver->getsolution() );
}

void sudEditor::RefreshProcCell(int x,int y){

	// currently there is no fast algorithm for evaluating the change of only one cell
	RefreshProcAll();
}



