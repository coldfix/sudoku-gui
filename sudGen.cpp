#include "sudGen.h"
#include "sudSolve.h"

#include <stdlib.h>		/* rand functions */
#include <time.h>		/*  */


#include "debug.h"


int getrand(int range){
	return rand()/2 % (range); }
void initrand(){
	static bool isseeded = 0;
	if(!isseeded){
		isseeded = true;
		srand(time(0)); }
}


template<class T>
T popval(GDynArray<T>& crds,int* pi=0){
	int i = getrand(crds.size());
	if(pi)
		*pi = i;
	T v = crds[i];
	crds.erase(i);
	return v; }
template<class T>
void pushval(GDynArray<T>&crds, T v){
	crds.append(v); }



struct geninfo{
	geninfo(int nbx,int nby, int difficulty,const GDynArray<int>* pattern=0)
		: riddle(nbx,nby), useallcrd(pattern!=0), crds(nbx*nby*nbx*nby)
	{
		if(pattern)
			crds = *pattern;
		else
			crds = GDynArray<int>::range( 0, riddle.getfieldnum() );

		switch(difficulty){
		case sudGen::difficulty_easy:
			proc = sudSolveBase::proc_simple|sudSolveBase::proc_assump;
			maxlev = minlev = 0;
			break;
		case sudGen::difficulty_medium:
			proc = sudSolveBase::proc_simple|sudSolveBase::proc_smart|sudSolveBase::proc_assump;
			maxlev = minlev = 0;
			break;
		case sudGen::difficulty_hard:
			proc = sudSolveBase::proc_simple|sudSolveBase::proc_smart
						|sudSolveBase::proc_clever|sudSolveBase::proc_assump;
			maxlev = 3;
			minlev = 0;
			break;
		}
	}

	geninfo(int nbx,int nby, int _proc,int _minlev,int _maxlev,const GDynArray<int>* pattern=0)
		: riddle(nbx,nby), useallcrd(pattern!=0), crds(nbx*nby*nbx*nby)
	{
		proc = _proc;
		maxlev = _maxlev;
		minlev = _minlev;
		if(pattern)
			crds = *pattern;
		else
			crds = GDynArray<int>::range( 0, riddle.getfieldnum() );
	}

	void reset(){
		crds = GDynArray<int>::range(0,riddle.getfieldnum());
		riddle.clear();
	}

	GDynArray<int> crds;
	bool useallcrd;			/* generate pattern oriented sudokus */
	sudSudoku riddle;
	int proc,maxlev,minlev;
};

bool ngenerate(geninfo*info){
	sudSolve solver(&info->riddle);
	solver.setproc(sudSolveBase::proc_simple|sudSolveBase::proc_smart);		// info->proc

	solver.test(0);
	if(!solver.isok())
		return false;

	if(solver.toohard() || !solver.isunique() ){
		static int debug_count =0;
		if(!info->crds.size())
			return 0;

		//GDynArray<int> popped( info->riddle.getfieldnum() );
		//for(int l=info->crds.size()<10?info->crds.size():10 ; l--;){
		//while(info->crds.size()){//
			DEBUG_MSG( ++debug_count%1000==0, wxString::Format(wxT("durchgaenge jetzt: %i"), debug_count) )

			int a = popval(info->crds);
			//popped.append(a);//
			sudSolveBase::MyValBatch possible = solver.getpossibilities(a);
			while(possible.size()){
				valtype v = popval(possible);
				info->riddle.set(a,v);
				if(ngenerate(info))
					return true;
			}
			info->riddle.set(a,0);
		//}//
		//info->crds.append(popped);//
		return false; }




	if(info->useallcrd){		// we do not try to force any difficulty setting in case we work with pattern
		for(int i=0; i<info->crds.size(); i++)
			info->riddle.set( info->crds[i], solver.getsolution().get(info->crds[i]) ) ;
		return true; }

	solver.setproc(info->proc);

	/*
	while( solver->getlevel() > info->maxlev && info->crds.size() ){
		int a = popval(info->crds);
		info->riddle.set( a, solver->getsolution().get(a) );
		solver->refresh(1);
	}*/
/*		// cannot be currently
	if(solver->getlevel()>info->maxlev){
		//return false;
		// we could return error to continue finding a solution,
		 //  but currently we just use the more difficult riddle then 
		return true;
	}*/

	GDynArray<int> as = GDynArray<int>::range(0,info->riddle.getfieldnum());
	as.rem( info->crds );

	while(as.size()){
		int a = popval(as);
		valtype ov = info->riddle.get(a);
		info->riddle.set(a,0);
		solver.test(info->maxlev);		/* koennte resourcen fressen ? */
		if( solver.toohard() || ! solver.isunique() )
			info->riddle.set(a,ov);
	}
	return true;
}


sudSudoku* sudGen::generate(int nbx,int nby,int difficulty){
	initrand();

//	return generate_old(nbx,nby,difficulty);


	geninfo info(nbx,nby,difficulty);
	while( ! ngenerate(&info) ){		/* achtung! */
		info.reset(); }
	return new sudSudoku(info.riddle); }



sudSudoku* sudGen::generateriddle(sudSudoku*sud,int minspots,int maxspots,
							int processing,bool bsmart,bool bclever){

	initrand();
	sudSudoku* rid = new sudSudoku(*sud);
	sudSolve solve(rid);

	int proc = sudSolveBase::proc_simple;
	if (bsmart)
		proc |= sudSolveBase::proc_smart;
	if(bclever)
		proc |= sudSolveBase::proc_clever;
	if(processing)
		proc |= sudSolveBase::proc_assump;
	solve.setproc( proc );

	GDynArray<int> as = GDynArray<int>::range( 0, sud->getfieldnum() );

	int chance = 1000 / (maxspots-minspots+1);

	while(as.size()){
		if(rid->getsolved()>=minspots && rid->getsolved()<=maxspots && getrand(1000)<chance
			|| rid->getsolved()==minspots)
			break;

		int a = popval(as);
		int ov = rid->get(a);
		rid->set(a,0);
		solve.test(processing);
		if( solve.toohard() || ! solve.isunique())
			rid->set(a,ov);
	}


	return rid; }

sudSudoku* sudGen::generatefull(int nbx,int nby){
	initrand();
	sudSudoku* sud = new sudSudoku(nbx,nby);
	sudSolve solve(sud);
//	solve.setproc( sudProcessing::proc_simple);
//	solve.setstoreany(true);

	GDynArray<int> as = GDynArray<int>::range( 0, sud->getfieldnum() );

	while( sud->getunsolved() ){
		solve.refresh(1);
		if( ! solve.isok() ){	/* this shouldn't often be the case normally, but it is possible :( */
			sud->clear();
			as = GDynArray<int>::range(0, sud->getfieldnum());	/* todo: not begin again but do some backtracikng? */
			continue; }
		if(solve.getnsolutions()>=1){
			sud->init(*solve.getsolutions().front());
			return sud; }

		int a = popval(as);
		valtype val = solve.getpossibilities(a)[ getrand(solve.getpossibilities(a).size()) ];
		sud->set(a,val);
	}

	return sud; }


sudSudoku* sudGen::generate(int nbx,int nby,
							int minspots,int maxspots,
							int processing,bool bsmart,bool bclever){
	sudSudoku* sud = generatefull(nbx,nby);
	sudSudoku* rid = generateriddle(sud,minspots,maxspots,processing,bsmart,bclever);
	delete sud;
	return rid; }


sudSudoku* sudGen::generate_old(int nbx, int nby, int difficulty){
	int fieldnum = nbx*nbx*nby*nby;

	int minspots, maxspots;

	if(nbx*nby == 9){
		switch(difficulty){
		case difficulty_easy:
			minspots = 25;
			maxspots = 35;
			break;
		case difficulty_medium:
			minspots = 27;
			maxspots = 19;
			break;
		case difficulty_hard:
			minspots = 0;
			maxspots = 18;
			break;
		};
	}else if(nbx*nby==6){
		switch(difficulty){
		case difficulty_easy:
			minspots = 12;
			maxspots = 15;
			break;
		case difficulty_medium:
			minspots = 10;
			maxspots = 12;
			break;
		case difficulty_hard:
			minspots = 0;
			maxspots = 10;
			break;
		};
	}else{		// difficulty computing will have to be improved....
		if(difficulty==difficulty_hard)
			minspots = 0;
		else
			minspots = fieldnum/(3+difficulty);
		maxspots = fieldnum/(1.5+difficulty/2.0);
	}

	int processings = difficulty>=difficulty_hard?2:0;
	bool bsmart = difficulty >= difficulty_medium;
	bool bclever = 0; //difficulty >= difficulty_hard;	// bclever kostet mehr rechenzeit, als dass es irgendetwas angeblich so "cleveres" nutzt
	return generate(nbx,nby, minspots,maxspots, processings, bsmart,bclever ); }

