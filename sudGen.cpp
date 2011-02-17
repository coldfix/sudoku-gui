#include <wx/wxprec.h>



#include "sudGen.h"
#include "sudSolve.h"

#include <stdlib.h>		/* rand functions */
#include <time.h>		/*  */


#include "debug.h"


void sudGen::Init()
{
	srand(time(0));
}

void sudGen::UnInit()
{
}


class genyielder : public Yielder
{
	sudGen* m_generator;
public:
	genyielder(sudGen* gen){
		m_generator = gen; }

	bool yield(){
		wxTheApp->Yield();
		return !m_generator->aborting(); }
};

int sudGen::gentime(){
	return (generated - started).GetMilliseconds().ToLong(); }

int sudGen::reducetime(){
	return (reduced - generated).GetMilliseconds().ToLong(); }

int sudGen::totaltime(){
	return (reduced - started).GetMilliseconds().ToLong(); }


bool sudGen::reduce(const Sudoku& sudoku,int minspots,int maxspots, int maxlevels, int difficulty){
	genyielder yielder(this);
	sudSolve solve(sudoku.bx(),sudoku.by());
	m_task = taskContinue;
	m_state = stateReducing;

	Sudoku riddle(sudoku.bx(),sudoku.by());
	int chance = 1000 / (maxspots-minspots+1);

	std::vector<int> as;
	as.reserve( riddle.size() );
	for(int a=0; a<riddle.size(); a++)
		as.push_back(a);
	while(!solve.solved() || riddle.getsolved()<minspots ){	// offene felder auswürfeln
		if(! as.size() )									// extremely unprobable
			break;
		int a;
		do{
			int ia = getrand(as.size());
			a = as[ia];
			as.erase(as.begin() + ia);
		} while( solve.solved(a) && !solve.solved() );
		riddle.set(a, sudoku[a]);
		solve.setsolution(a,sudoku[a]);
	}

	if( riddle.getsolved()>= minspots )		// reduce riddle according to difficulty level
	{
		as.clear();
		for(int a=0;a<riddle.size();a++)
			if(riddle[a])
				as.push_back(a);

		while(as.size()){
			if(!yielder.yield())
				break;

			int ia=getrand(as.size());
			int a=as[ia];
			as.erase(as.begin() + ia);
			riddle.set(a, 0);
			solve.init(riddle);
			solve.test(maxlevels);
			wxASSERT_MSG(!solve.error(),"\n\nUnexpected failure: error occured while solving generated sudoku!\n\n" );

			if(!solve.solved() || solve.difficulty()>difficulty){
				riddle.set(a, sudoku[a]);
				continue; }
			if(solve.difficulty()==difficulty && riddle.getsolved()<=maxspots && getrand(1000)<chance)
				break;
			if(riddle.getsolved()<=minspots)
				break;
		}
	}

	bool success = !aborting();
	if(success)
		m_riddle = riddle;
	m_state = stateIdle;
	return success; }



bool sudGen::generatefull(int nbx,int nby){
	genyielder yielder(this);
	sudSolve solve(nbx,nby);
	m_task = taskContinue;
	m_state = stateGenerating;
	for(int f=0;f<solve.high();f++){					// 1. block vorausfüllen
		int a = solve.a_bf(0,f);
		int iv = getrand(solve.possibilities(a));
		solve.setsolution( a, solve.possibility(a, iv)); }
//	for( int y=solve.fy(); y<solve.high(); y++ ){		// 1. spalte ausfüllen
//		int a = solve.a_xy(0,y);
//		int iv = getrand(solve.possibilities(a));
//		solve.setsolution( a, solve.possibility(a, iv)); }
	solve.fill(&yielder);
	bool success = !aborting();
	if( success )
		m_full = solve.sudoku();
	m_state = stateIdle;
	return success; }

bool sudGen::generate(int nbx,int nby, int minspots,int maxspots, int maxlevels,int difficulty){
	m_task = taskContinue;
	bool success;
	while(m_task == taskContinue){
		started = wxDateTime::UNow();
		success = generatefull(nbx,nby);
		generated = wxDateTime::UNow();
		if(!success)
			continue;
		success = reduce(GetFull(),minspots,maxspots,maxlevels,difficulty);
		reduced = wxDateTime::UNow();
		if(success)
			break;
	}
	return success; }


bool sudGen::generate(int nbx, int nby, int difficulty){
	int high = nbx*nby, size = high*high;
	int minspots, maxspots;
	if(high == 9){
		switch(difficulty){
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
		};
	}else if(high==6){
		switch(difficulty){
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
		};
	}else{		// difficulty computing will have to be improved....
		minspots = size/(3.5 - difficulty*0.5);
		maxspots = size/(3.0 - difficulty*0.5);
	}
	int maxlevels = 0;
	if(difficulty==difficulty_hard){
		if( nbx*nby <= 9 )
			maxlevels = 6;
	/*	else if(nbx*nby == 12)
			maxlevels = 4;
		else if(nbx*nby<=20)
			maxlevels = 2;*/
		else
			maxlevels = 1;
	}
	return generate(nbx,nby, minspots,maxspots, maxlevels, difficulty ); }

