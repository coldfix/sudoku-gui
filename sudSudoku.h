#pragma once
#include "crd.h"


/// usefull defines

#define notype

#define walk_t(tcrd,tcrd_ctor,type)		for( type tcrd = tcrd_ctor;tcrd--; ){
#define except(exception)						if( (exception) ){ continue; }
#define endwalk							} \

#define cwalkcrd(tcrd,tcrd_ctor,_type)	walk_t(tcrd,tcrd_ctor(-1),_type)	/* use of 'notype' means, that variable has to be declares before using */
#define walkint(i,high)					walk_t(i,high,int)
#define walkcrd(tcrd,tcrd_ctor)			walk_t(tcrd,tcrd_ctor(-1),int)

#define cwalkrowof(_xcrd,pobject)	cwalkcrd(_xcrd,(pobject)->xcrd,XCrd)
#define cwalkcolof(_ycrd,pobject)	cwalkcrd(_ycrd,(pobject)->ycrd,YCrd)
#define cwalkboxof(_fcrd,pobject)	cwalkcrd(_fcrd,(pobject)->fcrd,FCrd)
#define cwalkboxesof(_bcrd,pobject)	cwalkcrd(_bcrd,(pobject)->bcrd,BCrd)
#define cwalkallof(_acrd,pobject)	cwalkcrd(_acrd,(pobject)->acrd,ACrd)

#define walkrowof(_xcrd,pobject)	walkcrd(_xcrd,(pobject)->xcrd)
#define walkcolof(_ycrd,pobject)	walkcrd(_ycrd,(pobject)->ycrd)
#define walkboxof(_fcrd,pobject)	walkcrd(_fcrd,(pobject)->fcrd)
#define walkboxesof(_bcrd,pobject)	walkcrd(_bcrd,(pobject)->bcrd)
#define walkallof(_acrd,pobject)	walkcrd(_acrd,(pobject)->acrd)

#define cwalkrow(_xcrd)				cwalkcrd(_xcrd,xcrd,XCrd)
#define cwalkcol(_ycrd)				cwalkcrd(_ycrd,ycrd,YCrd)
#define cwalkbox(_fcrd)				cwalkcrd(_fcrd,fcrd,FCrd)
#define cwalkboxes(_bcrd)			cwalkcrd(_bcrd,bcrd,BCrd)
#define cwalkall(_acrd)				cwalkcrd(_acrd,acrd,ACrd)

#define walkrow(_xcrd)				walkcrd(_xcrd,xcrd)
#define walkcol(_ycrd)				walkcrd(_ycrd,ycrd)
#define walkbox(_fcrd)				walkcrd(_fcrd,fcrd)
#define walkboxes(_bcrd)			walkcrd(_bcrd,bcrd)
#define walkall(_acrd)				walkcrd(_acrd,acrd)



/// important types

typedef unsigned short valtype;


/*
	class sudDynSize
		> represents the dimensions of a sudoku
		> dimensions can be set at runtime
*/

class sudDynSize{
public:
	sudDynSize(void);
	sudDynSize(int _nbx,int _nby);
	sudDynSize(const sudDynSize&);
	~sudDynSize();


	/// dimensions access

	bool setsize(int _nbx,int _nby);
	bool initialized() const			{ return nbx ? true : false; }

	int b_x() const						{ return nbx; }
	int b_y() const						{ return nby; }
	int f_x() const						{ return nfx; }
	int f_y() const						{ return nfy; }

	valtype getmaxval() const			{ return highest; }
	valtype getminval() const			{ return 1; }

	int gethigh() const					{ return highest; }

	int getfieldnum() const				{ return highest*highest; }


	/// coord-convertion-tools

	typedef DynCrd ACrd, XCrd, YCrd, BCrd, FCrd, FrCrd, BrCrd;

	ACrd acrd_xy(int x,int y) const		{ return acrd( x, y ); }
	XCrd xcrd_a(int a) const			{ return xcrd( acrd(a).getx() ); }
	YCrd ycrd_a(int a) const			{ return ycrd( acrd(a).gety() ); }

	ACrd acrd_bf(int b,int f) const		{ return acrd_xy( xcrd_bf(b,f), ycrd_bf(b,f) ); }
	BCrd bcrd_a(int a) const			{ return bcrd( xcrd_a(a).gethigh(), ycrd_a(a).gethigh() ); }
	FCrd fcrd_a(int a) const			{ return fcrd( xcrd_a(a).getlow(), ycrd_a(a).getlow() ); }

	BCrd bcrd_xy(int x,int y) const		{ return bcrd_a( acrd_xy(x,y) ); }
	FCrd fcrd_xy(int x,int y) const		{ return fcrd_a( acrd_xy(x,y) ); }
	XCrd xcrd_bf(int b,int f) const		{ return xcrd( bcrd(b).getx(), fcrd(f).getx() ); }
	YCrd ycrd_bf(int b,int f) const		{ return ycrd( bcrd(b).gety(), fcrd(f).gety() ); }


	ACrd acrd_yx(int y,int x) const		{ return acrd_xy(x,y); }
	ACrd acrd_fb(int f,int b) const		{ return acrd_bf(b,f); }

	BCrd bcrd_yx(int y,int x) const		{ return bcrd_xy(x,y); }
	FCrd fcrd_yx(int y,int x) const		{ return fcrd_xy(x,y); }

	XCrd xcrd_fb(int f,int b) const		{ return xcrd_bf(b,f); }
	YCrd ycrd_fb(int f,int b) const		{ return ycrd_bf(b,f); }

	ACrd acrd_frb(int fr,int b) const	{ return acrd_bf(b,frcrd(fr).reverse()); }
	YCrd ycrd_frb(int fr,int b) const	{ return ycrd_bf(b,frcrd(fr).reverse()); }
	XCrd xcrd_frb(int fr,int b) const	{ return xcrd_bf(b,frcrd(fr).reverse()); }

protected:
	int nbx,nby,
		nfx,nfy,
		highest;

public:		// currently public. BUT NOT TO BE MODIFIED MANUALLY!

					/*  abbr: xib = x in box, yob = y of box, ...  */
	DynCrd  acrd,		// pattern for xy-coord - high/low: x/y
			xcrd,		// pattern for x-coord - xob/xib
			ycrd,		// pattern for y-coord - yob/yib
			bcrd,		// pattern for box-coord - xob/yob
			fcrd,		// patt f coord within box - xib/yib
			frcrd,		//							- yib/xib
			brcrd;		//							- yob/xob
};



/*
	class sudSudoku
		> represents a sudoku with n_box_x * n_box_y boxes with each n_box_y*n_box_x fields
		> size can be set at runtime

	we dont provide a statSudoku class because the sudoku class isn't used in performance-sensitive context
*/

class sudSudoku : public sudDynSize{
public:

	/// construction & destruction

	sudSudoku();
	sudSudoku(int n_box_x,int n_box_y,valtype*data=0);
	sudSudoku(const sudSudoku&sd);
	~sudSudoku(void);


	/// (re-)initialization

	void clear();
	bool setsize(int n_box_x,int n_box_y);
	bool init(int n_box_x,int n_box_y,valtype* data);
	void init( const sudSudoku& sd )					{ init( sd.nbx,sd.nby, sd.pdata ); }
	bool init( valtype* data )							{ init( nbx, nby, data ); }


	/// test

	bool possible(int x,int y,valtype test)  const		{ return possible( acrd_xy(x,y), test ); }
	bool possible(int a, valtype test)  const;

	int getunsolved() const								{ return unsolved; }
	int getsolved() const								{ return getfieldnum()-getunsolved(); }

	bool possible(const sudSudoku&sud) const;


	bool okay() const;


	bool equal(const sudSudoku& sud) const{
		if(b_x()!=sud.b_x() || b_y()!=sud.b_y())
			return false;
		walkall(xi)
			if( get(xi) != sud.get(xi) )
				return false;
		endwalk
		return true; }
	bool operator==(const sudSudoku&sud) const{
		return equal(sud); }


	/// coord-access


	bool set(int a, valtype num);
	bool set(int x, int y, valtype num)					{ return set(acrd_xy(x,y),num); }
	int get(int a) const								{ return pdata[a]; }
	int get(int x,int y) const							{ return get(acrd_xy(x,y)); }


	template<class stream>
	stream& save( stream& str ) const{
		str << nbx << wxT(" ") << nby << wxT("\n");
		walkcol( yi )
			YCrd y = ycrd(gethigh()-yi-1);
			walkrow( xi )
				XCrd x = xcrd(gethigh()-xi-1);
				str << get(x,y);
				if(x.getlow()!=x.getmax()-1){
					if(x.getlow()==x.lowlimit()-1)
						str << wxT("  ");
					str << wxT(" ");
				}
			endwalk
			if( y.getlow() != y.getmax()-1){
				str << wxT("\n");
				if( y.getlow() == y.lowlimit()-1 )
					str << wxT("\n");
			}
		endwalk
		return str; }

	template< class stream >
	stream& load( stream& str ){
		int xcol, ycol;
		str >> xcol >> ycol;
		init(xcol,ycol,0);
		walkcol( yi )
			walkrow( xi )
				int cur;
				str >> cur;
				if(!set(gethigh()-xi-1,gethigh()-yi-1,cur))
					return str;
			endwalk
		endwalk
		return str; }

protected:
	/// members
	valtype* pdata;
	int unsolved;
};




/*
	sudStatSize
		> static version of sudDynSize
		> size of a sudoku with at compile time known dimensions
		> performance feats for frequent operations (eg sudoku solvation or generation)
*/


template<int _nbx,int _nby>
struct sudStatSize{

/// types & consts

	enum{	nbx = _nbx,
			nby = _nby,
			nfx = nby,
			nfy = nbx,
			highest = nbx*nby,
			fieldnum = highest*highest };

					/*  abbr: xib = x in box, yob = y of box, ...  */
	typedef StatCrd<nbx*nfx,nby*nfy>	acrd,ACrd;		// pattern for xy-coord - high/low: x/y
	typedef StatCrd<nbx,nfx>			xcrd,XCrd;		// pattern for x-coord - xob/xib
	typedef StatCrd<nby,nfy>			ycrd,YCrd;		// pattern for y-coord - yob/yib
	typedef StatCrd<nbx,nby>			bcrd,BCrd;		// pattern for box-coord - xob/yob
	typedef StatCrd<nfx,nfy>			fcrd,FCrd;		// patt f coord within box - xib/yib
	typedef StatCrd<nby,nbx>			brcrd,BrCrd;	//							- yib/xib	bcrd::RevCrd
	typedef StatCrd<nfy,nfx>			frcrd,FrCrd;	//							- yob/xob	fcrd::RevCrd


/// dimensions access

	static int b_x() 						{ return nbx; }
	static int b_y() 						{ return nby; }
	static int f_x() 						{ return nfx; }
	static int f_y() 						{ return nfy; }

	static valtype getmaxval() 			{ return highest; }
	static valtype getminval() 			{ return 1; }

	static int gethigh() 					{ return highest; }

	static int getfieldnum() 				{ return fieldnum; }


/// coord-convertion-tools

	static ACrd acrd_v(int v)			{ return acrd(v); }
	static ACrd acrd_hl(int h,int l)	{ return acrd(h,l); }
	static XCrd xcrd_v(int v)			{ return xcrd(v); }
	static XCrd xcrd_hl(int h,int l)	{ return xcrd(h,l); }
	static YCrd ycrd_v(int v)			{ return ycrd(v); }
	static YCrd ycrd_hl(int h,int l)	{ return ycrd(h,l); }
	static BCrd bcrd_v(int v)			{ return bcrd(v); }
	static BCrd bcrd_hl(int h,int l)	{ return bcrd(h,l); }
	static FCrd fcrd_v(int v)			{ return fcrd(v); }
	static FCrd fcrd_hl(int h,int l)	{ return fcrd(h,l); }

	static ACrd acrd_xy(int x,int y)	{ return acrd( x, y ); }
	static XCrd xcrd_a(int a)			{ return xcrd( acrd(a).getx() ); }
	static YCrd ycrd_a(int a)			{ return ycrd( acrd(a).gety() ); }

	static ACrd acrd_bf(int b,int f)	{ return acrd_xy( xcrd_bf(b,f), ycrd_bf(b,f) ); }
	static BCrd bcrd_a(int a)			{ return bcrd( xcrd_a(a).gethigh(), ycrd_a(a).gethigh() ); }
	static FCrd fcrd_a(int a)			{ return fcrd( xcrd_a(a).getlow(), ycrd_a(a).getlow() ); }

	static BCrd bcrd_xy(int x,int y)	{ return bcrd_a( acrd_xy(x,y) ); }
	static FCrd fcrd_xy(int x,int y)	{ return fcrd_a( acrd_xy(x,y) ); }
	static XCrd xcrd_bf(int b,int f)	{ return xcrd( bcrd(b).getx(), fcrd(f).getx() ); }
	static YCrd ycrd_bf(int b,int f)	{ return ycrd( bcrd(b).gety(), fcrd(f).gety() ); }


	static ACrd acrd_yx(int y,int x)	{ return acrd_xy(x,y); }
	static ACrd acrd_fb(int f,int b)	{ return acrd_bf(b,f); }

	static BCrd bcrd_yx(int y,int x)	{ return bcrd_xy(x,y); }
	static FCrd fcrd_yx(int y,int x)	{ return fcrd_xy(x,y); }

	static XCrd xcrd_fb(int f,int b)	{ return xcrd_bf(b,f); }
	static YCrd ycrd_fb(int f,int b)	{ return ycrd_bf(b,f); }

	static ACrd acrd_frb(int fr,int b)	{ return acrd_bf(b,frcrd(fr).reverse()); }
	static YCrd ycrd_frb(int fr,int b)	{ return ycrd_bf(b,frcrd(fr).reverse()); }
	static XCrd xcrd_frb(int fr,int b)	{ return xcrd_bf(b,frcrd(fr).reverse()); }

};