#pragma once

#include "Sudoku.h"

#include <wx/datetime.h>

class sudGen{
public:
	enum{ difficulty_easy,difficulty_medium,difficulty_hard };

	static void Init();
	static void UnInit();

	sudGen(const Sudoku& riddle);
	sudGen(int nbx = 3, int nby = 3);

	void cancel(){
		m_state = stateAborting;
		m_task = taskAbort; }

	void stop(){
		m_state = stateStopping;
		m_task = taskStop; }

	void restart(){
		m_state = stateAborting;
		m_task = taskContinue; }

	bool stopping() const { return m_state == stateStopping; }
	bool aborting() const { return m_state == stateAborting; }
	bool finished() const { return m_state == stateIdle; }
	bool working() const { return m_state == stateGenerating || m_state == stateReducing; }

	void setdim(int nbx, int nby);
	void setriddle(const Sudoku&);

	bool fill();
	bool reduce(int minspots, int maxspots, int maxlevels, int difficulty);

	bool generate(int minspots,int maxspots,int maxlevels,int difficulty);
	bool generate(int difficulty);

	const Sudoku& GetSudoku() const { return m_sudoku; }

	int gentime() const { return m_gentime; }
	int reducetime() const { return m_redtime; }
	int totaltime() const { return gentime() + reducetime(); }

protected:
	Sudoku m_sudoku;

	int m_task, m_state, m_gentime, m_redtime;

	enum{
		stateIdle, stateGenerating, stateReducing, stateAborting, stateStopping,
		taskContinue, taskAbort, taskStop
	};
};

