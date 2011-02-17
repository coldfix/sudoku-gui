#include <wx/wxprec.h>

#include "sudGen.h"
#include "sudSolve.h"

#include <stdlib.h>		/* rand functions */
#include <time.h>		/*  */
#include <algorithm>	/* tsd::generate */ 
#include <vector>


/*
*/

template<class T>
T remove_random(std::vector<T>& v)
{
	int i = getrand(v.size());
	int elem = v[i];
	std::swap(v[i], v[v.size()-1]);
	v.pop_back();
	return elem;
}

class IntSequence {
	int counter;
public:
	IntSequence (int start = 0) : counter(start) { }

	int operator() () {
		return counter++; }
};

class genyielder : public Yielder
{
	sudGen* m_generator;
	int m_count;
public:
	genyielder(sudGen* gen)
	{
		m_count = 0;
		m_generator = gen;
	}

	bool yield()
	{
		if (++m_count % 15 == 0)
			wxTheApp->Yield();
		return !m_generator->aborting() && !m_generator->stopping();
	}
};




/*
*/


void sudGen::Init()
{
	srand(time(0));
}

void sudGen::UnInit()
{
}


sudGen::sudGen(const Sudoku& riddle)
	: m_sudoku(riddle)
{
	m_gentime = m_redtime = 0;
}

sudGen::sudGen(int nbx, int nby)
	: m_sudoku(nbx, nby)
{
	m_gentime = m_redtime = 0;
}


void sudGen::setriddle(const Sudoku& sudoku)
{
	m_sudoku = sudoku;
	m_gentime = m_redtime = 0;
}

void sudGen::setdim(int nbx, int nby)
{
	m_sudoku.setsize(nbx, nby);
	m_gentime = m_redtime = 0;
}


bool sudGen::reduce(int minspots, int maxspots,
					int maxlevels,
					int difficulty)
{
	const Sudoku& sudoku = m_sudoku;

	m_task = taskContinue;
	m_state = stateReducing;
	wxDateTime start = wxDateTime::UNow();
	genyielder yielder(this);

	sudSolve solve(sudoku.bx(), sudoku.by());
	Sudoku riddle(sudoku.bx(), sudoku.by());
	int chance = 1000 / (maxspots - minspots + 1) * 20;

	std::vector<int> as(riddle.size());
	std::generate(as.begin(), as.end(), IntSequence(0));

	while (!solve.solved() || riddle.getsolved() < minspots)	// offene felder auswürfeln
	{
		if (!as.size())
			break;

		int a = remove_random(as);
		if (solve.solved(a) || !sudoku[a])
			continue;

		riddle.set(a, sudoku[a]);
		solve.setsolution(a, sudoku[a]);

		if (solve.error()){
			m_gentime += (wxDateTime::UNow() - start).GetMilliseconds().ToLong();
			m_state = stateIdle;
			return false; }
	}

	if (riddle.getsolved() >= minspots)		// reduce riddle according to difficulty level
	{
		as.clear();
		as.reserve(riddle.getsolved());
		for (int a = 0; a < riddle.size(); a++)
			if (riddle[a])
				as.push_back(a);

		while (as.size() && riddle.getsolved() > minspots)
		{
			if (!yielder.yield())
				break;

			int a = remove_random(as);
			riddle.set(a, 0);
			solve.init(riddle);
			solve.test(maxlevels);

			if (!solve.solved() || solve.difficulty() > difficulty)
			{
				riddle.set(a, sudoku[a]);
				continue;
			}

			if (solve.difficulty() == difficulty
				&& riddle.getsolved() <= maxspots
				&& getrand(1000) < chance)
			{
				break;
			}
		}
	}

	bool success = !aborting();
	if (success)
		m_sudoku = riddle;

	m_redtime = (wxDateTime::UNow() - start).GetMilliseconds().ToLong();
	m_state = stateIdle;
	return success;
}



bool sudGen::fill()
{
	m_task = taskContinue;
	m_state = stateGenerating;
	wxDateTime start = wxDateTime::UNow();
	genyielder yielder(this);
	sudSolve solve(m_sudoku);

	if (solve.unsolved() == solve.size())
	{
		for (int f = 0; f < solve.high(); f++)					// 1. block vorausfüllen
		{
			if (solve.error()) {
				m_gentime += (wxDateTime::UNow() - start).GetMilliseconds().ToLong();
				m_state = stateIdle;
				return false; }

			int a = solve.a_bf(0, f);
			int iv = getrand(solve.possibilities(a));
			solve.setsolution(a, solve.possibility(a, iv));
		}

	//	for (int y=solve.fy(); y < solve.high(); y++)				// 1. spalte ausfüllen
	//	{
	//		int a = solve.a_xy(0,y);
	//		int iv = getrand(solve.possibilities(a));
	//		solve.setsolution(a, solve.possibility(a, iv));
	//	}

	}

	solve.fill(&yielder);
	bool success = !aborting();
	if (success)
		m_sudoku = solve.sudoku();

	m_gentime += (wxDateTime::UNow() - start).GetMilliseconds().ToLong();
	m_state = stateIdle;
	return success;
}

bool sudGen::generate(int minspots, int maxspots,
					  int maxlevels,
					  int difficulty)
{
	m_task = taskContinue;
	bool success = true;
	m_gentime = m_redtime = 0;
	while (m_task == taskContinue)
	{
		success = fill();
		if (m_task != taskContinue)
			break;

		success = reduce(minspots, maxspots,
						 maxlevels,
						 difficulty);

		if (success)
			break;
	}
	return success;
}




bool sudGen::generate(int difficulty)
{
	int nbx = m_sudoku.bx(), nby = m_sudoku.by(),
		high = nbx*nby, size = high*high;
	int minspots, maxspots;
	if (high == 9)
	{
		switch (difficulty)
		{
			case difficulty_hard:
				minspots = 15;
				maxspots = 26;
				break;
			case difficulty_medium:
				minspots = 27;
				maxspots = 34;
				break;
			case difficulty_easy:
				minspots = 35;
				maxspots = 44;
				break;
		}
	}
	
	else if (high == 6)
	{
		switch (difficulty)
		{
			case difficulty_hard:
				minspots = 4;
				maxspots = 9;
				break;
			case difficulty_medium:
				minspots = 10;
				maxspots = 13;
				break;
			case difficulty_easy:
				minspots = 14;
				maxspots = 18;
				break;
		}
	}
	
	else				// difficulty computing will have to be improved....
	{
		minspots = size / (2.1 + difficulty * 0.5);
		maxspots = size / (1.2 + difficulty * 0.5);
	}

	int maxlevels = 0;
	if (difficulty == difficulty_hard)
	{
		if (nbx*nby <= 9)
			maxlevels = 6;
	/*	else if (nbx*nby == 12)
			maxlevels = 4;
		else if (nbx*nby <= 20)
			maxlevels = 2;*/
		else
			maxlevels = 1;
	}

	return generate(minspots, maxspots,
					maxlevels,
					difficulty);
}

