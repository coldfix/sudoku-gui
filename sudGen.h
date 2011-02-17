#pragma once

#include "sudSudoku.h"


class sudGen{
public:
	enum{ difficulty_easy,difficulty_medium,difficulty_hard };
	
	static sudSudoku* generateriddle(sudSudoku*,int minspots,int maxspots,int processing,bool bsmart,bool bclever);										// this function generates a sudoku-riddle out of a valid solvable sudoku
	static sudSudoku* generatefull(int nbx,int nby);									//
	static sudSudoku* generate(int nbx,int nby,int minspots,int maxspots,int processing,bool bsmart,bool bclever);
	static sudSudoku* generate_old(int nbx,int nby,int difficulty);
	static sudSudoku* generate(int nbx,int nby,int difficulty);
};

