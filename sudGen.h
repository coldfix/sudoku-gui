#pragma once

#include "Sudoku.h"

#include <wx/datetime.h>

class sudGen{
public:
	enum{ difficulty_easy,difficulty_medium,difficulty_hard };

	static void Init();
	static void UnInit();

	void cancel(){
		m_state = stateAborting;
		m_task = taskAbort; }

	void restart(){
		m_state = stateAborting;
		m_task = taskContinue; }

	bool aborting() const { return m_state==stateAborting; }
	bool finished() const { return m_state==stateIdle; }
	bool working() const { return m_state==stateGenerating || m_state==stateReducing; }

	bool generatefull(int nbx,int nby);

	bool reduce(const Sudoku&, int minspots, int maxspots, int maxlevels, int difficulty);

	bool generate(int nbx,int nby,int minspots,int maxspots,int maxlevels,int difficulty);
	bool generate(int nbx,int nby,int difficulty);

	const Sudoku& GetFull() const { return m_full; }
	const Sudoku& GetRiddle() const { return m_riddle; }

	int gentime();
	int reducetime();
	int totaltime();

protected:
	Sudoku m_full,m_riddle;

	wxDateTime started,generated,reduced;
	int m_task, m_state;

	enum{
		stateIdle, stateGenerating, stateReducing, stateAborting,
		taskContinue, taskAbort
	};
};

