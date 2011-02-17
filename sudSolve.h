#pragma once

#include "Sudoku.h"

#include <vector>



extern int getrand(int range, int min);
extern int getrand(int range);

struct Yielder {
	virtual bool yield() = 0;
};


typedef std::vector<valtype> valarray;



class sudSolve : public sudSize
{
public:

	/// consts & types

	enum{ state_error = 0 };

	enum{ difficulty_easy,
		difficulty_medium,
		difficulty_hard };


	///

	sudSolve();
	sudSolve(int nbx,int nby);
	explicit sudSolve(const Sudoku&);
	~sudSolve();
	void init(const Sudoku&);


	void resize(int _nbx, int _nby, bool clear=false);

	void setsize(int _nbx, int _nby);

	void solve(int maxlev,Yielder* =0);						// solve until error / ambiguous
	void test(int maxlev,Yielder* =0);
	void fill(Yielder* =0);

	void deepthoughts(bool smart,bool clever);
	void thinksmart();
	void thinkclever();

	///

	bool dirty() const { return m_dirty; }

	int unsolved() const { return m_unsolved; }

	bool solved() const{ return unsolved() == 0; }

	bool error() const{ return m_error; }
	bool ok() const { return ! error(); }					//the processing which has been done has lead to no error


	bool hassolution() const { return m_hassolution; }		//at least one solution has been found
	bool unique() const { return m_unique; }				//the sudoku is solvable and there is only one solution that has been found. 
	bool toohard() const { return m_toohard; }				//sudSolve couldn't make sure there is no other way to solve the sudoku than the found solutions
	int usedlevel() const { return m_iusedlevel; }			//

	int difficulty() const { return m_difficulty; }
	bool smart() const { return m_smart; }
	bool clever() const { return m_clever; }
	bool multiple() const { return m_multiple; }


	///

	bool error(int a) const{
		return m_states[a] == state_error || solved(a) && !possible(a,solution(a)); }
	bool error(int x, int y) const{
		return error(a_xy(x,y)); }

	bool ok(int a) const{
		return ! error(a); }
	bool ok(int x, int y) const{
		return ok(a_xy(x,y)); }

	bool solved(int a) const{
		return m_states[a] > 0; }
	bool solved(int x, int y) const{
		return solved(a_xy(x,y)); }

	bool possible(int a, int val) const{
		return valfield(val)[a]; }
	bool possible(int x, int y, int val) const{
		return possible(a_xy(x,y),val); }

	int solution(int a) const{
		return m_states[a]; }
	bool solution(int x, int y) const{
		return solution(a_xy(x,y)); }

	int possibilities(int a) const{
		return m_states[a] > 0 ? 1 : -m_states[a]; }
	int possibilities(int x, int y) const{
		return possibilities(a_xy(x,y)); }

	void getpossible(int a, valarray& v) const;
	int possibility(int a, int i) const;

	void setsolution(int a, int val);
	void discard(int a, int val);

	bool* valfield(int val) const{
		return m_possible + (val-1) * size(); }
	bool* operator[](int val) const{
		return valfield(val); }

	Sudoku sudoku() const;


	/// internal
protected:
	int datasize() const{
		return high()*size(); }

	enum{ method_findany, method_findunique, method_test };

	void solve_(int method,int maxlevels,Yielder*);

	/// members

	signed char * m_states;
	bool * m_possible, m_error, m_dirty, m_smart,m_clever;
	int m_unsolved;

	bool m_toohard, m_unique, m_hassolution, m_multiple, m_immediate;
	int m_iusedlevel, m_difficulty;
};


