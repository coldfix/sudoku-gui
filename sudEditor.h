#pragma once
#include "Sudoku.h"
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

	const sudSolve& GetSolver() const				{ return m_solver; }

	const Sudoku* GetRiddle() const				{ return &m_riddle; }
	const Sudoku* GetUser() const				{ return &m_user; }
	const Sudoku* GetSolution() const			{ return &m_solution; }
	const sudSize* GetSize() const					{ return GetRiddle(); }

	void ClearSudoku()								{ SetSize( GetSize()->bx(), GetSize()->by() ); }

	int GetRiddle(int x,int y) const				{ return GetRiddle()->get(x,y); }
	int GetUser(int x,int y) const					{ return GetUser()->get(x,y); }
	int GetSolution(int x,int y) const				{ return GetSolution()->get(x,y); }
	bool IsFalseInput(int x,int y) const;
	bool IsFalseInput(int x,int y,int val) const;

	virtual void SetShowSolution(bool on);
	bool GetShowSolution() const					{ return m_showsolution; }

	virtual bool SetSize(int nbx,int nby);
	virtual bool SetRiddle(const Sudoku&sriddl);

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

	sudSolve m_solver;
	bool m_initmode,m_showsolution;
	int m_refreshlev;
	Sudoku m_riddle,m_user,m_solution;
};
