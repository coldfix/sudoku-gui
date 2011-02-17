#pragma once

#include"gdynarray.h"
#include "sudSudoku.h"


class sudSudoku;


class sudSolveBase{
public:
	/// consts & types

	enum{ pr_invalid=-1, pr_noteval, pr_solution, pr_sevsolution, pr_toohard=4 };

	enum{ df_maxlev=5,		//default maximum number uf levels
		df_maxunsolved=70,  //maximum number of unsolved fields, when beginning to assume
		df_maxch=7 };       //maximum number of choices in assume-field

	enum{ proc_simple=0,	//proc_simple is always ON. it is essential for the algorithm
		proc_smart=1,		//proc_smart represents a somewhat smarter way of knowing how to solve sudokus
		proc_clever=2,		//proc_clever represents an even an extra bit smarter way of solving sudokus
		proc_assump=4 };	//proc_assump represents the way of solving sudokus by guessing fields and see to what it leads


	typedef GArrayPointer<valtype> MyValView;
	typedef MyValView *MyPosField;
	typedef const MyValView *ConstMyPosField;
	typedef GDynArray<valtype> MyValBatch,valbatch;
	typedef GDynArray<sudSudoku*> MySolutionArray;

	virtual ~sudSolveBase() {}


	/// solve and preference interface

	virtual void setmaxlevel(int)=0;				//max number of assumptions
	virtual void setriddle(const sudSudoku*)=0;		//initialize
	virtual void setproc(int)=0;					//set usable algorithms (for generation of sudokus with difficulty levels, this is the opportunity to turn off some elements of the assumption algorithm)
	virtual int getproc() const=0;					//get usable algorithms
	virtual void refresh(int lev=df_maxlev)=0;		//will try to find all possible solutions.. this is usefull to know afterwards more about possibilities
	virtual void test(int lev=df_maxlev)=0;			//will only see if sudoku is valid, if valid: unique, toohard - might save some cpu time



	/// after solvation

	virtual const MySolutionArray& getsolutions() const=0;		// all solutions that have been found
	virtual int getnsolutions() const {
		return getsolutions().size(); }
	virtual const sudSudoku& getsolution() const=0;				//direct/unique solution

	virtual int getprocneeded() const=0;						//get used algorithms
	virtual int getunsolved() const =0;							//number of unsolved fields
	virtual bool issolved() const{ return !getunsolved(); }		//there are no unsolved fields

	virtual bool isok() const=0;		//the processing which has been done has lead to no error
	virtual bool hassolution() const=0;	//at least one solution has been found
	virtual bool isunique() const=0;	//the sudoku is solvable and there is only one solution that has been found. 
	virtual bool toohard() const=0;		//sudSolve couldn't make sure there is no other way to solve the sudoku than the found solutions
	virtual int getlevel() const=0;		//

	virtual ConstMyPosField getposfield() const =0;				// get the possibility array		/* const does not const the contents */
	virtual const MyValView& getpossibilities(int a) const{		// possibilities of one field
		return getposfield()[a]; }

	///
	virtual void setusedlevel(int l)=0;

};


/*
	sudSolver class declaration > wrapper class for processings
*/

class sudSolve : public sudSolveBase{
public:

	/// construction & clean up

	sudSolve(const sudSudoku* priddle);					// we take a sudSudoku riddle as argument
	~sudSolve();


	/// solve and preference interface

	void refresh(int lev=df_maxlev);
	void test(int lev=df_maxlev);

	void setriddle(const sudSudoku* sud) { m_riddle = sud; }		/* different behaviour than in sudSolveBase: riddle has to stay valid as long as sudSolve sudSolve object exists and no other riddle is set! */
	const sudSudoku* getriddle() const	{ return m_riddle; }

	void setproc(int proc)				{ m_proc = proc; }
	int getproc() const					{ return m_proc; }

	void setmaxlevel(int lev)				{ m_maxlev = lev; }

	sudSolveBase* getchild() const		{ return m_processing; }


	/// access to solutions & current processing

	int getunsolved() const			{ return getsolution().getunsolved(); }
	bool isok() const				{ return getchild()->isok(); }
	bool toohard() const			{ return getchild()->toohard(); }
	bool isunique() const			{ return getchild()->isunique(); }
	bool hassolution() const		{ return getchild()->hassolution(); }
	int getlevel() const			{ return getchild()->getlevel(); }

	const MySolutionArray& getsolutions() const	{ return getchild()->getsolutions(); }
	ConstMyPosField getposfield() const		{ return getchild()->getposfield(); }
	const sudSudoku& getsolution() const	{ return getchild()->getsolution(); }			// if the sudoku has no solution or more than one, we only want to get the best sudoku that is possible

	int getprocneeded() const				{ return getchild()->getprocneeded(); }

	void setusedlevel(int l)		{ getchild()->setusedlevel(l); }
protected:
	void prepare(int nmaxl);

	int m_maxlev,m_proc;
	int m_dimx,m_dimy;
	const sudSudoku* m_riddle;
	sudSolveBase *m_processing;
};



