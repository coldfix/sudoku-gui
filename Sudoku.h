#ifndef __SUDOKU_H__INCLUDED__
#define __SUDOKU_H__INCLUDED__


/*
	class sudSize
		> represents the dimensions of a sudoku
		> dimensions can be set at runtime
*/

class sudSize{
public:
	/// construction

	sudSize(void)					{ init(0,0); }
	sudSize(int bx,int by)			{ init(bx,by); }
	sudSize(const sudSize&size)		{ init(size.bx(),size.by()); }
	virtual ~sudSize() {}

protected:
	void init(int _nbx,int _nby){
		nbx = _nbx;
		nby = _nby;
		row = nbx * nby;
		total = row * row; }
public:

	/// size

	void setsize(const sudSize&size){
		setsize(size.bx(), size.by()); }

	virtual void setsize(int bx,int by){
		init(bx,by); }

	int bx() const					{ return nbx; }
	int by() const					{ return nby; }
	int fx() const					{ return nby; }
	int fy() const					{ return nbx; }
	int high() const				{ return row; }
	int size() const				{ return total; }

	bool initialized() const		{ return bx()&&by(); }

	int maxval() const				{ return row; }
	int minval() const				{ return 1; }

	bool operator ==(const sudSize& ds) const{
		return bx()==ds.bx() && by()==ds.by(); }


	/// coord transformations

	int a_xy(int x,int y) const		{ return x*high() + y; }
	int a_bf(int b,int f) const		{ return a_xy( x_bf(b,f), y_bf(b,f) ); }

	int x_a(int a) const			{ return a/high(); }
	int y_a(int a) const			{ return a%high(); }
	int b_a(int a) const			{ return b_hl(xb_a(a),yb_a(a)); }
	int f_a(int a) const			{ return f_hl(xf_a(a),yf_a(a)); }

	int x_bf(int b,int f) const		{ return x_hl(xb_b(b),xf_f(f)); }
	int y_bf(int b,int f) const		{ return y_hl(yb_b(b),yf_f(f)); }
	int b_xy(int x,int y) const		{ return b_hl(xb_x(x),yb_y(y)); }
	int f_xy(int x,int y) const		{ return f_hl(xf_x(x),yf_y(y)); }

	int x_hl(int h,int l) const		{ return h*fx() + l; }
	int y_hl(int h,int l) const		{ return h*fy() + l; }
	int b_hl(int h,int l) const		{ return h*by() + l; }
	int f_hl(int h,int l) const		{ return h*fy() + l; }

	int xb_x(int x) const			{ return x/fx(); }
	int xf_x(int x) const			{ return x%fx(); }
	int yb_y(int y) const			{ return y/fy(); }
	int yf_y(int y) const			{ return y%fy(); }

	int xb_b(int b) const			{ return b/by(); }
	int xf_f(int f) const			{ return f/fy(); }
	int yb_b(int b) const			{ return b%by(); }
	int yf_f(int f) const			{ return f%fy(); }

	int xb_a(int a) const			{ return xb_x(x_a(a)); }
	int xf_a(int a) const			{ return xf_x(x_a(a)); }
	int yb_a(int a) const			{ return yb_y(y_a(a)); }
	int yf_a(int a) const			{ return yf_y(y_a(a)); }


	typedef int (*const sudSize::u_a)(int a) ;
	typedef int (*const sudSize::a_uv)(int a) ;
protected:

	/// members

	int nbx,nby,row,total;
};



/*
	class Sudoku
		> represents a sudoku with n_box_x * n_box_y boxes with each n_box_y*n_box_x fields
		> size can be set at runtime

	we dont provide a statSudoku class because the sudoku class isn't used in performance-sensitive context
*/

template<class T>
class SudokuT : public sudSize{
public:

	/// construction & destruction

	SudokuT()
	{
		setsize(0,0);
	}

	SudokuT(int _nbx,int _nby,const T*data=0)
	{
		clear(_nbx,_nby,data);
	}

	SudokuT(const SudokuT&sd)
	{
		*this = sd;
	}

	~SudokuT(void){
		if(initialized())
			delete [] m_data;
	}


	/// (re-)initialization

	void clear(){
		if(initialized())
			for(int a=0;a<size();a++)
				m_data[a] = 0;
		m_unsolved = size(); }

	void clear(int _nbx,int _nby, const T* data=0){
		bool realloc = _nbx*_nby != high();
		if(initialized() && realloc)
			delete [] m_data;
		sudSize::setsize(_nbx,_nby);
		if(initialized()&&realloc)
			m_data = new T[size()];
		if(initialized()&&data){
			m_unsolved = size();
			for(int a=0; a<size(); a++)
				if(m_data[a] = data[a])
					--m_unsolved;
		}else
			clear();
	}

	void setsize(int _nbx,int _nby){
		clear(_nbx,_nby); }

	void setsize(const sudSize& size){
		setsize(size.bx(),size.by()); }

	SudokuT& operator=(const SudokuT& sudoku){
		clear(sudoku.nbx, sudoku.nby, sudoku.m_data);
		return *this; }


	/// test

	int getunsolved() const								{ return m_unsolved; }
	int getsolved() const								{ return size()-getunsolved(); }


	bool equals(const SudokuT& sud) const{
		if(bx()!=sud.bx() || by()!=sud.by())
			return false;
		for(int a=0;a<size();a++)
			if( get(a) != sud.get(a) )
				return false;
		return true; }

	bool operator==(const SudokuT&sud) const{
		return equals(sud); }


	/// coord-access

	const T& get(int a) const{
		return m_data[a]; }

	const T& get(int x,int y) const{
		return get(a_xy(x,y)); }

	const T& operator [](int a) const{
		return get(a); }


	void set(int a, const T& num){
		if(num && !m_data[a])
			--m_unsolved;
		else if(!num&&m_data[a])
			++m_unsolved;
		m_data[a] = num; }

	void set(int x, int y, const T& num){
		set(a_xy(x,y),num); }


	template<class stream>
	stream& save( stream& str ) const{
		str << bx() << wxT(" ") << by() << wxT("\n");
		for( int y=0; y< high(); y++ ){
			if(y)
				str << (y%fy() ? wxT("\n") : wxT("\n\n"));
			for( int x=0; x< high(); x++ ){
				if(x)
					str << (x%fx() ? wxT(" ") : wxT("  "));
				str << get(x,y);
			}
		}
		return str; }

	template< class stream >
	stream& load( stream& str ){
		int xcol, ycol;
		str >> xcol >> ycol;
		setsize(xcol,ycol);
		for( int y=0; y<high(); y++ ){
			for(int x=0;x<high(); x++){
				int cur;
				str >> cur;
				set(x,y, cur);
			}
		}
		return str; }

protected:
	/// members
	T* m_data;
	int m_unsolved;
};




/*

*/


typedef unsigned char valtype;
typedef SudokuT<valtype> Sudoku;




#endif // __SUDOKU_H__INCLUDED__
