#pragma once


/*
	class StatCrd		lowlimit and highlimit must be known at compile time. performance feats
	class DynCrd		lowlimit and highlimit can be set at run time. performance flaws, flex feats


	Purpose:	crd-classes represents a coord within a two-dimensional array
				so it consists of two int-parts which can be expressed as a single offset
*/


template<int highlimit,int lowlimit=highlimit>
class StatCrd{

	/// members & internal methods
	int i;
	void initv(int v)						{ i = v; }
public:

	/// types & consts

	enum{ lh = highlimit, lx=lh, ll = lowlimit, ly=lowlimit, max=lx*ly };

	typedef StatCrd<lowlimit,highlimit> RevCrd;


	/// construction & destruction

	StatCrd(void)								{ initv(0); }
	StatCrd(int v)								{ set(v); }
	StatCrd(int h,int l)						{ set(h,l); }
	StatCrd(const StatCrd&crd)					{ set(crd); }
	~StatCrd()									{ }

	RevCrd reverse()							{ return RevCrd(getlow(),gethigh()); }

	StatCrd operator()(int v) const				{ return StatCrd(v); }
	StatCrd operator()(int h,int l) const		{ return StatCrd(h,l); }


	/// modification

	void set(int val)							{ initv( test(val) ? val : getmax() ); }
	void set(int high,int low)					{ set(high*lowlimit()+low); }
	void set(const StatCrd& crd)				{ initv(crd.get()); }

	void setlow(int low)						{ set(gethigh(),low); }
	void sety(int y)							{ setlow(y); }

	void sethigh(int high)						{ set(high,getlow()); }
	void setx(int x)							{ sethigh(x); }

	void verify()								{ set(get()); }


	/// accessors

	static int highlimit()						{ return lh; }
	static int xlimit()							{ return highlimit(); }
	static int lowlimit()						{ return ll; }
	static int ylimit()							{ return lowlimit(); }

	static int getmax()							{ return highlimit()*lowlimit(); }
	static bool test(int val)					{ return val >= 0 && val < getmax(); }

	bool ok() const								{ return test(get()); }
	operator bool() const						{ return ok(); }


	int get() const								{ return i; }
	int geta() const							{ return get(); }
	int a() const								{ return geta(); }
	operator int() const						{ return get(); }

	int gethigh() const							{ return get()/lowlimit(); }
	int h() const								{ return gethigh(); }
	int getx() const							{ return gethigh(); }
	int x() const								{ return getx(); }

	int getlow() const							{ return get()%lowlimit(); }
	int l() const								{ return getlow(); }
	int gety() const							{ return getlow(); }
	int y() const								{ return gety(); }


	/// modification operators

	StatCrd&operator++()						{ ++i; return *this; }		// die neue variante verzichtet auf error control bei ++,-- erhöhen, vermindern! .. also kein: set(get()+1)
	StatCrd operator++(int)						{ return StatCrd(i++); }

	StatCrd&operator--()						{ --i; return *this; }
	StatCrd operator--(int)						{ return StatCrd(i--); }

	StatCrd&operator+=(int val)					{ set(get()+val); return *this; }
	StatCrd&operator-=(int val)					{ set(get()-val); return *this; }

	StatCrd operator+(int val)					{ StatCrd(*this,i+val); }
	StatCrd operator-(int val)					{ StatCrd(*this,i-val); }

	StatCrd& operator=(int val)					{ set(val); return *this; }
	StatCrd& operator=(const StatCrd& crd)		{ set(crd); return *this; }


	/// comparison
	bool operator==(int val)const				{ return val==get(); }
	bool operator==(const StatCrd& crd) const	{ return crd.get()==get(); }
	bool operator!=(int val)const				{ return !(*this==val); }
	bool operator!=(const StatCrd& crd)const	{ return !(*this==crd); }

};	// class StatCrd

template<int h,int l> bool operator==(int a,const StatCrd<h,l>& crd)	{ return crd==a; }
template<int h,int l> bool operator!=(int a,const StatCrd<h,l>& crd)	{ return crd!=a; }


class DynCrd{

	/// members & internal methods

	int i,lh,ll;

	void initl(int nh,int nl)					{ lh = nh; ll = nl; }
	void initv(int v)							{ i = v; }
	void inithl(int h,int l)					{ initv(h*lowlimit()+l); }

public:
	/// construction & destruction

	DynCrd(void)								{ initl(0,1); initv(0); }
	DynCrd(int nh,int nl)						{ initl(nh,nl); initv(getmax()); }
	DynCrd(int nh,int nl,int v)					{ initl(nh,nl); set(v); }
	DynCrd(int lh,int ll,int h,int l)			{ initl(lh,ll); set(h,l); }
	DynCrd(const DynCrd&patt,int v)				{ set(patt,v); }
	DynCrd(const DynCrd&patt,int h,int l)		{ set(patt,h,l); }
	DynCrd(const DynCrd&crd)					{ set(crd); }
	~DynCrd()									{ }

	DynCrd reverse()							{ return DynCrd(lowlimit(),highlimit(),getlow(),gethigh()); }

	DynCrd operator()(int v) const				{ return DynCrd(*this,v); }
	DynCrd operator()(int h,int l) const		{ return DynCrd(*this,h,l); }


	/// modification
	void set(int val)							{ initv( test(val) ? val : getmax() ); }
	void set(int high,int low)					{ set(high*lowlimit()+low); }
	void set(const DynCrd& crd)					{ initl(crd.highlimit(),crd.lowlimit()); initv(crd.get()); }

	void setlow(int low)						{ set(gethigh(),low); }
	void sety(int y)							{ setlow(y); }

	void sethigh(int high)						{ set(high,getlow()); }
	void setx(int x)							{ sethigh(x); }

	void set(const DynCrd&patt,int v)			{ initl(patt.highlimit(),patt.lowlimit()); set(v); }
	void set(const DynCrd&patt,int h,int l)		{ initl(patt.highlimit(),patt.lowlimit()); set(h,l); }

	void setlimits(int nhigh,int nlow)			{ initl(nhigh,nlow); set( get() ); }

	void verify()								{ set(get()); }


	/// accessors

	int highlimit() const						{ return lh; }
	int xlimit() const							{ return highlimit(); }
	int lowlimit() const						{ return ll; }
	int ylimit() const							{ return lowlimit(); }

	int getmax() const							{ return highlimit()*lowlimit(); }
	bool test(int val) const					{ return val >= 0 && val < getmax(); }

	bool ok() const								{ return test(get()); }
	operator bool() const						{ return ok(); }


	int get() const								{ return i; }
	int geta() const							{ return get(); }
	int a() const								{ return geta(); }
	operator int() const						{ return get(); }

	int gethigh() const							{ return get()/lowlimit(); }
	int h() const								{ return gethigh(); }
	int getx() const							{ return gethigh(); }
	int x() const								{ return getx(); }

	int getlow() const							{ return get()%lowlimit(); }
	int l() const								{ return getlow(); }
	int gety() const							{ return getlow(); }
	int y() const								{ return gety(); }


	/// modification operators

	DynCrd&operator++()							{ ++i; return *this; }
	DynCrd operator++(int)						{ return DynCrd(*this,++i); }

	DynCrd&operator--()							{ --i; return *this; }
	DynCrd operator--(int)						{ return DynCrd(*this,--i); }

	DynCrd&operator+=(int val)					{ set(get()+val); return *this; }
	DynCrd&operator-=(int val)					{ set(get()-val); return *this; }

	DynCrd operator+(int val)					{ DynCrd(*this, i+val); }
	DynCrd operator-(int val)					{ DynCrd(*this, i-val); }

	DynCrd& operator=(int val)					{ set(val); return *this; }
	DynCrd& operator=(const DynCrd& crd)		{ set(crd); return *this; }


	/// comparison

	bool operator==(int val)const				{ return val==get(); }
	bool operator==(const DynCrd& crd) const	{ return crd.get()==get(); }
	bool operator!=(int val)const				{ return ! (*this==val); }
	bool operator!=(const DynCrd& crd)const		{ return ! (*this==crd); }

};	// class DynCrd

/*
bool operator==(int val,const DynCrd& crd)		{ return crd==val; }
bool operator!=(int val,const DynCrd& crd)		{ return crd!=val; }
*/
