#include "sudSudoku.h"

#include "debug.h"



/*
	sudDynSize
*/

	/// construction | destruction

sudDynSize::sudDynSize(void)
{
	setsize(0,0);
}

sudDynSize::sudDynSize(int _nbx,int _nby)
{
	setsize(_nbx,_nby);
}

sudDynSize::sudDynSize(const sudDynSize& size)
{
	setsize( size.nbx, size.nby );
}

sudDynSize::~sudDynSize()
{
}

	/// dimensions access

bool sudDynSize::setsize(int _nbx,int _nby){

	nfy = nbx = _nbx;
	nfx = nby = _nby;
	highest = nfx * nfy;

	if(!_nbx || !_nby)
		return false;

	xcrd.setlimits( nbx, nfx );
	ycrd.setlimits( nby, nfy );
	acrd.setlimits( nbx*nfx, nby*nfy );
	bcrd.setlimits( nbx, nby );
	fcrd.setlimits( nfx, nfy );
	frcrd.setlimits( nfy, nfx );
	brcrd.setlimits( nby, nbx );
	return true; }


/*
	sudSudoku
*/

/// construction & destruction

sudSudoku::sudSudoku(void)
{
	DEBUG_INC_COUNT(sudSudoku)
	pdata = 0;
	init(1,1,0);
}

sudSudoku::sudSudoku(int nbox_x,int nbox_y,valtype*data)
{
	DEBUG_INC_COUNT(sudSudoku)
	pdata = 0;
	init(nbox_x,nbox_y,data);
}

sudSudoku::sudSudoku(const sudSudoku&sd)
{
	DEBUG_INC_COUNT(sudSudoku)
	pdata = 0;
	init(sd);
}

sudSudoku::~sudSudoku(void)
{
	DEBUG_DEC_COUNT(sudSudoku)
	delete [] pdata;
}


/// (re-)initialization

bool sudSudoku::setsize( int _nbx, int _nby ){
	int oldfn = getfieldnum();
	sudDynSize::setsize(_nbx,_nby);
	if(oldfn!=getfieldnum()){
		delete [] pdata;
		pdata = new valtype[getfieldnum()];
	}
	clear();
	return true; }

void sudSudoku::clear(){
	int ai;
	walkall(ai)
		set( ai, 0 );
	endwalk
	unsolved = getfieldnum(); }


bool sudSudoku::init( int nbox_x, int nbox_y, valtype* data ){
	if( !nbox_x || !nbox_y )
		return 0;
	setsize(nbox_x,nbox_y);
	int ai;
	if(data){
		walkall(ai)
			set( ai, data[ai] );
		endwalk }
	return true; }

/// test

			// this simple routine only checks if any of the basic rules are hurt
			// it does not do any detailed error output
			// more detailed error output should be provided by the solver.. 
bool sudSudoku::okay() const{
	if(!pdata || !b_x())
		return false;
	int val;
	walkall(ai) except(!(val = get(ai)))
		int x = xcrd_a(ai), y=ycrd_a(ai), b=bcrd_a(ai), f=fcrd_a(ai);
		walkint(xi,x)
			if( get(xi,y)==val )
				return false;
		endwalk
		walkint(yi,y)
			if( get(x,yi)==val )
				return false;
		endwalk
		walkint(fi,f)
			if( get(acrd_bf(b,fi))==val )
				return false;
		endwalk
	endwalk
	return true; }

	/* having one solution this function can test whether a user input fits into the solution */
bool sudSudoku::possible(const sudSudoku&sud) const{
	if(b_x()!=sud.b_x() || b_y()!=sud.b_y())
		return false;
	walkall(ai)
		if( sud.get(ai)&&get(ai) && sud.get(ai)!=get(ai) )
			return false;
	endwalk
	return true; }

/// coord-access

bool sudSudoku::set(int a, valtype num){
	if(num&&!pdata[a])
		--unsolved;
	else if(!num&&pdata[a])
		++unsolved;
	pdata[a] = num;
	return true; }

/// test value

bool sudSudoku::possible(int a, valtype test) const{
	if(test==0)
		return true;
	if(test < getminval() || test > getmaxval() )		// value to high/low ?
		return false;
	DynCrd x = xcrd_a(a), y = ycrd_a(a);
	int xi,yi,fi;
	walkrow(xi) except(x==xi)
		if(get(xi,y)== test)
			return false;
	endwalk
	walkcol(yi) except(y==yi)
		if(get(x,yi)==test)
			return false;
	endwalk
	DynCrd b = bcrd_a(a),f = fcrd_a(a);
	walkbox(fi) except( f==fi )
		if(get(acrd_bf(b,fi))==test)
			return false;
	endwalk
	return true; }

