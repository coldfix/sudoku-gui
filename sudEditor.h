#pragma once
#include "sudSudoku.h"
#include "sudSolve.h"

class sudSolve;


class sudEditor{
public:

	enum{
		cellmode_riddle, cellmode_user_correct, cellmode_user_incorrect,cellmode_solution,
		_cellmodes = cellmode_riddle|cellmode_user_correct|cellmode_user_incorrect|cellmode_solution,
		cellmode_invalid = 0x10
	};

	sudEditor(int nbx,int nby);
	~sudEditor();

	sudSolve* GetSolver() const						{ return m_solver; }
	const sudSolveBase::MyValView& GetPossibilities(int x,int y) const;

	const sudSudoku* GetRiddle() const				{ return &m_riddle; }
	const sudSudoku* GetUser() const				{ return &m_user; }
	const sudSudoku* GetSolution() const			{ return &m_solution; }
	const sudDynSize* GetSize() const				{ return GetRiddle(); }

	void ClearSudoku()								{ SetSize( GetSize()->b_x(), GetSize()->b_y() ); }

	int GetRiddle(int x,int y) const				{ return GetRiddle()->get(x,y); }
	int GetUser(int x,int y) const					{ return GetUser()->get(x,y); }
	int GetSolution(int x,int y) const				{ return GetSolution()->get(x,y); }
	bool IsFalseInput(int x,int y) const;
	bool IsFalseInput(int x,int y,int val) const;

	virtual void SetShowSolution(bool on);
	bool GetShowSolution() const					{ return m_showsolution; }

	virtual bool SetSize(int nbx,int nby);
	virtual bool SetRiddle(const sudSudoku&sriddl);

	virtual int GetUnsolved() const;

	virtual int GetCellMode(int x, int y) const;

	virtual bool EnterInitMode(bool on);
	bool IsInitMode() const							{ return m_initmode; }

	bool IsReadOnly(int x,int y) const				{ return !IsInitMode() && GetRiddle(x,y); }

	virtual bool SetCell(int x,int y,int val);
	virtual int GetCell(int x,int y) const;

	int GetRefreshLevels() const					{ return m_refreshlev; }
	void SetRefreshLevels(int l)					{ m_refreshlev = l; }


protected:
	virtual void RefreshProcAll();
	virtual void RefreshProcCell(int x,int y);

	sudSolve* m_solver;
	bool m_initmode,m_showsolution;
	int m_refreshlev;
	sudSudoku m_riddle,m_user,m_solution;
};
