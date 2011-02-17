#include "sudSolve.h"

#include "debug.h"


template<class T>
void pa_clear( GArrayPointer<T*>& pa ){
	for(int i=0; i<pa.size(); i++){
		delete pa.at(i);}
	pa.clear();
}



template<class sizeclass>
class sudSolveImpl : public sudSolveBase, protected sizeclass{
public:
	/// types & consts	

	enum fstats{ fnopossible=0, fsolved, fsevpos };		// all values which are greater than 1 mean the same as fsevpos

	typedef sizeclass MySize;


protected:
	/// members
	MyPosField m_posfld;
	bool m_proc_state, m_error_state,m_solution_created;
	bool m_ok,m_toohard,m_unique,m_hassolution;
	int m_maxlev,m_iusedlevel,m_unsolved,m_proc,m_procneeded;
	mutable sudSudoku m_solution;
	MySolutionArray m_solutions;
	sudSolveImpl *m_child;

public:
	/// construction / destruction
	sudSolveImpl(const MySize& size)
		: m_maxlev(0),	/* level initialization has to be done by deriving class... necessary to avoid pure virtual function call (CreateChild) */
		m_iusedlevel(0), MySize(size), m_solutions(15),
		m_solution( size.b_x(), size.b_y() ),
		m_solution_created(0)
	{
		DEBUG_INC_COUNT(sudSolveImpl)
		seterrorstate(0);
		m_posfld = MyValView::CreateArray(getfieldnum(),gethigh());
		setproc(proc_simple|proc_smart|proc_clever|proc_assump);
	}

	virtual ~sudSolveImpl(){
		DEBUG_DEC_COUNT(sudSolveImpl)
		MyValView::DestroyArray(m_posfld);
		if(HasChild())
			delete m_child;
		pa_clear(m_solutions);
	}

	/// after solve

	int getunsolved() const							{ return m_unsolved; }
	int getprocneeded() const{
		return m_procneeded; }

	bool isok() const				{ return m_ok; }
	bool toohard() const			{ return m_toohard; }
	bool hassolution() const		{ return m_hassolution; }
	bool isunique() const			{ return m_unique; }

	int getlevel() const			{ return m_iusedlevel; }

	ConstMyPosField getposfield() const			{ return m_posfld; }

	int getnsolutions() const						{ return m_solutions.size(); }
	const MySolutionArray& getsolutions() const		{ return m_solutions; }

	const sudSudoku& getsolution() const{
		if(m_solution_created)
			return m_solution;
		walkall(ai)
			m_solution.set(ai, solution(ai));
		endwalk
		return m_solution; }


	/// child

	virtual sudSolveImpl* CreateChild()=0;
	bool HasChild() const							{ return m_maxlev>0; }
	sudSolveImpl* GetChild() const					{ return m_child; }


	/// initialization

	//
	void setmaxlevel(int lev){
		bool old = HasChild();
		m_maxlev = lev;
		if( HasChild() ){
			if(!old)
				m_child = CreateChild();	/* this is the reason, level initialization cannot be done correctly in the constructor */
			m_child->setmaxlevel(m_maxlev-1);
		}else if(old)
			delete m_child;
	}

	//
	void setriddle(const sudSudoku*r){
		seterrorstate(0);
		m_solution_created =0;
		m_procneeded = proc_simple;

		MyValBatch bv = MyValBatch::full(getmaxval());
		walkall( ai )					// initialize all fields
			if( r->get( ai ) ){
				ref(ai).assign(1,r->get(ai));
			}else
				ref(ai) = bv;
		endwalk
			DEBUG_MSG( ! r->okay(), wxString::Format(wxT("Invalid sudoku as parameter\nOn level: %i"),m_maxlev) )

		m_unsolved = getfieldnum();		// begin updating
		walkall( ai ) except( ! r->get(ai) )
			if(post_change(ai)==evt::invalid){
//				DEBUG_MSG(1,wxString::Format(wxT("Error during initialization: init from sudoku\non level: %i"),m_maxlev))		/*of debug interest*/
				false;	/* we do not have to interact, since the post solution routine already sets the error_state */
			}
		endwalk
	}

	//
	void init_pvals(sudSolveImpl*pr,int a, valtype val){
		m_solution_created =0;
		seterrorstate(0);
		m_procneeded = proc_simple;
		if(pr){
			m_unsolved = pr->getunsolved();
			MyValView::CopyArray(getposfield(),pr->getposfield(),getfieldnum());
			DEBUG_MSG( !MyValView::CmpArray(getposfield(),pr->getposfield(), getfieldnum() ), wxT("falsch kopiert") )
		}
		if( ! acrd(a).ok() )
			post_reduced(0);
		else{					// forward the assumption to the event system:
			DEBUG_MSG( solution(a),
					wxString::Format(wxT("schon gesetzt!!\n\npos: %i\nVal hier: %i\nVal parent: %i\nassump: %i"),
					a,solution(a),pr->solution(a),val) )
			if(solution(a))		/* this normally should not be possible */
				++m_unsolved;
			ref(a) = MyValBatch(1,val);
			if(post_change(a)==evt::invalid)
				false;	/* see before */
		}
	}

	//
	void setproc(int pr){
		m_proc = pr;
		if(HasChild())
			GetChild()->setproc(pr); }
	int getproc() const{
		return m_proc; }


	void setusedlevel(int l){
		m_iusedlevel = l; }


	/// solvation

	void refresh(int lev){
//		seterrorstate(0);		/* we cannot reset the error state here: it was set in the setriddle/init_pvals methods! */
		setmaxlevel(lev);
		setusedlevel(0);
		solve();
	}
	void test(int lev){
//		seterrorstate(0);		/* we cannot reset the error state here: it was set in the setriddle/init_pvals methods! */
		setmaxlevel(lev);
		setusedlevel(0);
		solve(1);
	}



protected:

	/// evt - processing

	struct evt{ enum{ reduced, solved, invalid }; };

	bool getevt(){
		bool ret = getstate() && !geterrorstate();
		setstate(0);
		return ret; }

	int post_solution( int a ){
		setstate(true);
		DEBUG_MSG(issolved(), wxT("falscher solution post: m_unsolved==0") )
		if(!--m_unsolved){
			setstate(0);
			const sudSudoku& sol = getsolution();
			if( ! sol.okay() ){
				DEBUG_MSG(1, wxT("solution not okay!!") )
				return evt::invalid; }
			m_solutions.append(new sudSudoku(sol));
			return evt::solved; }
		return update_solved(a) ? evt::solved : evt::invalid; }

	int post_reduced( int a ){
		DEBUG_MSG(solution(a), wxT("falscher reduce post: solved") )
//		DEBUG_MSG(getunsolved()==1, wxT("falscher reduce post: m_unsolved==1") )	/* this is not an error */
		setstate(true);
		return evt::reduced; }

	int post_error( int a ){
		setstate(0);
		seterrorstate(1);
		return evt::invalid; }

	int post_change( int a ){
		if( cardinality(a) == fsolved )
			return post_solution(a);
		else if( cardinality(a) >= fsevpos )
			return post_reduced( a );
		else
			return post_error(a); }

		// state
	bool getstate() const{
		return m_proc_state; }
	void setstate(bool bstate){
		m_proc_state = bstate; }
	bool geterrorstate() const		{ return m_error_state; }
	void seterrorstate(bool es)		{ m_error_state = es; }

	int cardinality(int a) const					{ return ref(a).size(); }


	///

	MyPosField getposfield(){
		return m_posfld; }
	const MyValView& ref(int a) const{
		return m_posfld[a]; }
	MyValView& ref(int a){
		return m_posfld[a]; }
	MyValView& ref(int x,int y){
		return ref( acrd_xy(x,y) ); }
	valtype solution(int a) const					{ return cardinality(a)==fsolved ? ref(a)[0] : 0; }
	valtype solution(int x,int y) const				{ return solution( acrd_xy(x,y) ); }


	/// internal solvation
	virtual void solve( bool btest=false ){
		m_solution_created =0;
		pa_clear(m_solutions);
		int retcode = update_all();		// process all changes by logic routine
		m_ok = retcode!=pr_invalid;
		m_hassolution = retcode==pr_solution;
		m_unique = retcode==pr_solution;
		m_toohard = retcode==pr_noteval;
		if(retcode != pr_noteval
			|| ! HasChild() || !(getproc()&proc_assump)	// if we are too deep in assumption hierarchy, just break up
			|| getunsolved() > df_maxunsolved ){		/* todo: dynamize df_maxunsolved */
			return; }

		m_procneeded |= proc_assump;
		int cardin=highest+1,ai=0;			// if sudoku is not solved, we will do some assumptions. first we look for a good target cell
		walkall(aii) except( solution(aii) )
			if( cardinality(aii) < cardin ){
				cardin = cardinality(ai = aii);
				if(cardin==2)					// there is no better cardinality than 2
					break;
			}
		endwalk
		if(cardin>df_maxch){				/* todo: dynamize df_maxch */
			/* m_toohard is already set */
			return;}
		DEBUG_MSG(cardin>highest || cardin<=1,
				  wxString(wxT("Keine gueltige stelle fur assumption gefunden!\nStelle: %i\nCardinality: %i"),
				  ai,cardin) )	// zu debug zwecken.. sollte eigtl nie passieren, weil nicht solved


		bool toohard = 0;
		sudSolveImpl* subproc = GetChild();
		for(int i=ref(ai).size(); i-- ; ){
			DEBUG_MSG( solution(ai), wxString::Format(wxT("Loesung in assumptionsschleife aufgetaucht!!\nLoesung: %i\nIndex: %i"),solution(ai),ai) )

			subproc->init_pvals(this,ai,ref(ai)[i]);
			subproc->solve(btest);
			if(subproc->toohard())
				toohard = 1;
			if( subproc->getlevel()+1 > getlevel() )
				setusedlevel(subproc->getlevel()+1);
			if( ! subproc->isok() ){					// the assumed number is not possible
				ref(ai).erase(i);						//  - so we can delete it from the pvals
				if(post_change(ai)==evt::invalid)		//  and re-calculate
					false;
				solve();					// we know more know, so we can update our processing
				return;						//	> this is something that will have to be improved.. might cost a lot of cpu time in some cases (frequent [de-]allocation of solutions)
			}else{
				if(subproc->getnsolutions() && getnsolutions()){
					m_solutions.append( new sudSudoku(* subproc->getsolutions().front()) );
					break; }	// btest: we can break if we found that the sudoku is either invalid, unique or too hard
				const MySolutionArray& solutions = subproc->getsolutions();
				for(MySolutionArray::const_iterator it=solutions.begin() ; it!=solutions.end(); ++it)
					m_solutions.append( new sudSudoku(**it) );
			}
			if(btest && toohard)
				break;
		}

		DEBUG_MSG( geterrorstate(), wxT("errorstate==true on end of solve()!") )
//		m_ok = ! geterrorstate();		/* should never be set */

		m_hassolution = m_solutions.size();
		m_unique = m_solutions.size()==1;
		m_toohard = toohard;

		if( !m_toohard && m_hassolution && !btest ){		/* necessary? */
			walkall(ai)
				ref(ai).clear();
				for(int i=0;i<getnsolutions();i++)
					ref(ai).appendunique(getsolutions()[i]->get(ai));
			endwalk
		}
	}



	/// updating


/*
	processes all processing, that can be done currently, returns false to indicate pr_invalid
*/
	virtual int update_all(){
		if(geterrorstate())
			return pr_invalid;
		if(issolved())
			return pr_solution;
		while( getevt() )
			if(!update_reduced())
				return pr_invalid;
		if( issolved() ){
			// storesolution();
			return pr_solution; }
		return pr_noteval; }

/*
	removes any pvals in the same box/row/col of the solution
*/
	virtual bool update_solved(int a){
		valtype val = solution(a);
		if(val<=0){
			DEBUG_MSG(1, wxT("falscher solve post: nicht gelöst"))
			return false; }
		int x=xcrd_a(a), y=ycrd_a(a), b=bcrd_a(a), f=fcrd_a(a);
		walkrow( xi ) except( xi==x /*|| solution(xi,y)*/ )				// remove double vals within row
			if( ref(xi,y).rem( val ) && post_change( acrd_xy(xi,y)) == evt::invalid )
				return false;
		endwalk
		walkcol( yi ) except( yi==y /*|| solution(x,yi)*/ )				// remove double vals within col
			if( ref(x,yi).rem(val) && post_change(acrd_xy(x,yi)) == evt::invalid )
				return false;
		endwalk

		if(b_x()!=1&&b_y()!=1){ //rauswerfen?
		walkbox( fi ) except( fi==f /*|| solution( acrd_bf(b,fi) )*/ )	// remove double vals within box
			if( ref(acrd_bf(b,fi)).rem(val) && post_change(acrd_bf(b,fi)) == evt::invalid )
				return false;
		endwalk
		}
		return true; }

/*
	takes advantage of smarter algorithms to solve the sudoku
*/
	virtual bool update_reduced(){
		MyValBatch v(highest);

		if(getproc()&proc_smart){
			if(issolved())
				return 1;

			m_procneeded |= proc_smart;
					//search for pvals that are possible in only one cell
		walkcol(y)			// within one row
			walkrow(xi) except( solution(xi,y) )
				v = ref(xi,y);
				walkrow(xii) except(xii==xi)
					v.rem(ref(xii,y));
				endwalk
				if(v.size()>0){
					if(v.size()>1){
						post_error(acrd_xy(xi,y));
						return 0; }
					ref(xi,y) = v;
					if(post_solution(acrd_xy(xi,y))==evt::invalid)
						return 0;
				}
			endwalk
		endwalk
		walkrow(x)			// within one col
			walkcol(yi) except( solution(x,yi) )
				v = ref(x,yi);
				walkcol( yii ) except( yii==yi )
					v.rem(ref(x,yii));
				endwalk
				if(v.size()>0){
					if(v.size()>1 ){
						post_error(acrd_xy(x,yi));
						return 0; }
					ref(x,yi) = v;
					if(post_solution(acrd_xy(x,yi))==evt::invalid)
						return 0;
				}
			endwalk
		endwalk
		if(b_x()!=1&&b_y()!=1){
		walkboxes(b)		// within one box
			walkbox(fi) except( solution( acrd_bf(b,fi) ) )
				v = ref( acrd_bf(b,fi) );
				walkbox(fii) except( fii==fi )
					v.rem(ref(acrd_bf(b,fii)) );
				endwalk
				if(v.size()>0){
					if(v.size()>1){
						post_error(acrd_bf(b,fi));
						return 0; }
					ref(acrd_bf(b,fi)) = v;
					if(post_solution(acrd_bf(b,fi))==evt::invalid)
						return 0;
				}
			endwalk
		endwalk
		}

		}		// endif(proc&proc_smart)

		if( !(getproc()&proc_clever) || getstate() || issolved() )
			return 1;

		m_procneeded |= proc_clever;

	//search pvals that are possible in only
		cwalkrow(x)			// one box's col within a col
			walkint(ybi, nby)
				v.clear();
				walkint( yfi, nfy )
					MyValView& vr = ref( x, ycrd(ybi,yfi) );
					v.appendunique( vr.begin(), vr.end() );
				endwalk
				cwalkcol( yi ) except( yi.gethigh() == ybi )
					v.rem( ref(x,yi) );
				endwalk
				if( v.size() > 0 ){	// and remove these vals in the rest of the box
					int b = bcrd(x.gethigh(),ybi);
					cwalkbox( fi ) except( fi.gethigh() == x.getlow() )
						if( ref(acrd_bf(b,fi)).rem(v) && post_change(acrd_bf(b,fi))==evt::invalid)
							return 0;
					endwalk
				}
			endwalk
		endwalk
		cwalkcol(y)				// in one box's row of a row
			walkint(xbi, nbx)
				v.clear();
				walkint( xfi, nfx )
					MyValView& vr = ref( xcrd(xbi,xfi), y );
					v.appendunique( vr.begin(), vr.end() );
				endwalk
				cwalkrow( xi ) except( xi.gethigh() == xbi )
					v.rem( ref(xi,y) );
				endwalk
				if( v.size() > 0 ){	// and remove these vals in the rest of the box
					int b = bcrd( xbi, y.gethigh() );
					cwalkbox( fi ) except( fi.getlow() == y.getlow() )
						if( ref(acrd_bf(b,fi)).rem(v) && post_change(acrd_bf(b,fi))==evt::invalid)
							return 0;
					endwalk
				}
			endwalk
		endwalk
		cwalkboxes(b)
			walkint( xfi, nfx )	// in one col of a box
				v.clear();
				walkint( yfi, nfy )
					MyValView& vr = ref( acrd_bf(b,fcrd(xfi,yfi)) );
					v.appendunique( vr.begin(), vr.end() );
				endwalk
				cwalkbox( fi ) except( fi.gethigh()== xfi )
					v.rem( ref( acrd_bf(b,fi) ) );
				endwalk
				if( v.size() > 0 ){	// and remove these vals in the rest of the col
					int x = xcrd(b.gethigh(),xfi);
					cwalkcol( yi ) except( yi.gethigh() == b.getlow() )
						if( ref(acrd_xy(x,yi)).rem(v) && post_change(acrd_xy(x,yi))==evt::invalid)
							return 0;
					endwalk
				}
			endwalk
			walkint( yfi, nfy )	// in one row of a box
				v.clear();
				walkint( xfi, nfx )
					MyValView& vr = ref( acrd_bf(b,fcrd(xfi,yfi)) );
					v.appendunique( vr.begin(), vr.end() );
				endwalk
				cwalkbox( fi ) except( fi.getlow()== yfi )
					v.rem( ref( acrd_bf(b,fi) ) );
				endwalk
				if( v.size() > 0 ){	// and remove these vals in the rest of the row
					int y = ycrd(b.getlow(),yfi);
					cwalkrow( xi ) except( xi.gethigh() == b.gethigh() )
						if( ref(acrd_xy(xi,y)).rem(v) && post_change(acrd_xy(xi,y))==evt::invalid)
							return 0;
					endwalk
				}
			endwalk
		endwalk
		return 1; }


};


/*
	implementation class of sudSolveImpl
		> works with dynamically known sizes
		> slow operations

	take care: changing a sudokus size during an objects lifetime is not supplied
*/

class sudDynProcessing
	: public sudSolveImpl< sudDynSize >
{
public:
	/// construction | destruction
	sudDynProcessing(const sudSudoku*sud,int mxlv)
		: sudSolveImpl(*(MySize*)sud)
	{
		setmaxlevel(mxlv);
		setriddle(sud);
	}
protected:
	sudDynProcessing(sudDynProcessing*pf)
		: sudSolveImpl(*(MySize*)pf)
	{
		setmaxlevel(pf->m_maxlev-1);
	}

	virtual sudSolveImpl* CreateChild(){
		return new sudDynProcessing(this); }
public:
	~sudDynProcessing()			{ }

};


/*
	sudStatProcessing
		> static version of sudDynProcessing. performance feats
*/

/*	*/
template<int nbx,int nby>
class sudStatProcessing
	: public sudSolveImpl< sudStatSize<nbx,nby> >
{
public:
	/// construction | destruction

	sudStatProcessing(const sudSudoku*sud,int mxlv)
		: sudSolveImpl(MySize())
	{
		setmaxlevel(mxlv);
		setriddle(sud);
	}
protected:
	sudStatProcessing(sudStatProcessing*pf)
		: sudSolveImpl(MySize())
	{
		setmaxlevel(pf->m_maxlev-1);
	}

	virtual sudSolveImpl* CreateChild(){
		return new sudStatProcessing(this); }
public:
	~sudStatProcessing()								{ }
};








/*
	the following abstracted functions are currently not in use due to performance reasons


	todo: try to save time by avoiding these function pointers
		> is there a possibility to use templates instead of function pointers?
		  these would make many inlines possible!!
	we could then take use of these functions again
*/

/* int sudSolve::update_reduced(int a){

	crd x=xcrd_a(a), y=ycrd_a(a);
	crd b=bcrd_a(a), f=fcrd_a(a);
	crd fr = f.reverse();

		//search for unique pvals ...
	if(   ! reduce_wcells_by_cell( a, xcrd, &sudSolve::xcrd_a,&sudSolve::ycrd_a,&sudSolve::acrd_xy )	// within one row
		|| !reduce_wcells_by_cell( a, ycrd, &sudSolve::ycrd_a,&sudSolve::xcrd_a,&sudSolve::acrd_yx )	// within one col
		|| !reduce_wcells_by_cell( a, bcrd, &sudSolve::bcrd_a,&sudSolve::fcrd_a,&sudSolve::acrd_bf )	// within one box
					//search possible vals that uniquely appear ...
		|| !reduce_ndis_by_wdis(y,x.gethigh(),xcrd,fcrd, &sudSolve::acrd_xy,&sudSolve::acrd_bf,&sudSolve::ycrd_bf,&sudSolve::bcrd_xy)		// in one box's row of a row -> remove them from the rest of the box 
		|| !reduce_ndis_by_wdis(x,y.gethigh(),ycrd,fcrd, &sudSolve::acrd_yx,&sudSolve::acrd_bf,&sudSolve::xcrd_bf,&sudSolve::bcrd_yx)		// in one box's col of a col -> remove them from the rest of the box 
		|| !reduce_ndis_by_wdis(b,f.gethigh(),fcrd,ycrd, &sudSolve::acrd_fb,&sudSolve::acrd_xy,&sudSolve::bcrd_xy,&sudSolve::xcrd_fb)		// in one col of a box -> remove them from the rest of the col
		|| !reduce_ndis_by_wdis(b,fr.gethigh(),frcrd,xcrd, &sudSolve::acrd_frb,&sudSolve::acrd_yx,&sudSolve::bcrd_yx,&sudSolve::ycrd_frb)	// in one row of a box -> remove them from the rest of the row
		){
	}
	return 0;
} */

/*

/// to be put into class declaration
			// currently not in use:
	typedef WCrd (sudSolve::*fWtor_a)(int) const;
	typedef VCrd (sudSolve::*fVtor_a)(int) const;
	typedef ACrd (sudSolve::*fAtor_wv)(int,int) const;
	typedef ACrd (sudSolve::*fAtor_nm)(int,int) const;
	typedef VCrd (sudSolve::*fVtor_nm)(int,int) const;
	typedef NCrd (sudSolve::*fNtor_wv )(int,int) const;

	bool reduce_wcells_by_cell(int a,crd wdim,fWtor_a,fVtor_a,fAtor_wv);
	bool reduce_ndis_by_wdis(int v,int wh,crd wdim,crd mdim,sudSolve::fAtor_wv Ator_wv,sudSolve::fAtor_nm Ator_nm,sudSolve::fVtor_nm Vtor_nm,sudSolve::fNtor_wv Ntor_wv );
/// end to be put into class declaration
*/

/*
bool sudSolve::reduce_wcells_by_cell(int a,
							   crd wdim,
							   sudSolve::fWtor_a Wtor,
							   sudSolve::fVtor_a Vtor,
							   sudSolve::fAtor_wv Ator ){
	crd w = (this->*Wtor)(a), v = (this->*Vtor)(a);
	valbatch z;

	walk( wi , wdim ) except( wi == w )
		z.clear();
		z = ref( (this->*Ator)(wi,v) );

		walk( wi2, wdim ) except( wi2==wi )
			z.rem(ref((this->*Ator)(wi2,v)));
		endwalk

		if(z.size()>=1){
			if(z.size()>1){
				post_error((this->*Ator)(wi,v));
				return false;
			}else{
				int asol = (this->*Ator)(wi,v);
				ref( asol ) = z;
				if( post_solution( asol ) == evt::invalid )
					return false;
			}
		}
	endwalk
	return true;
}
*/

/*
bool sudSolve::reduce_ndis_by_wdis(int v,
							 int wh,
							 crd wdim,
							 crd mdim,
							 sudSolve::fAtor_wv Ator_wv,
							 sudSolve::fAtor_nm Ator_nm,
							 sudSolve::fVtor_nm Vtor_nm,
							 sudSolve::fNtor_wv Ntor_wv ){

	valbatch z;
	walkint( wh2, wdim.highlimit() ) except( wh2==wh )		// we are excluding wh here, because it is not possible that vals are appearing in wh and nowhere else, when wh is the place where vals were deleted
		z.clear();
		walkint( wl, wdim.lowlimit() )						// load all pvals in current row/col/box/box
			valbatch& y = ref( (this->*Ator_wv)(wdim(wh2,wl),v) );
			z.insert(z.end(), y.begin(), y.end() );
		endwalk

		walk(wi,wdim) except( wi.gethigh()==wh2 )			// rem all vals appearing in the rest of the row/col/box/box
			rem(z,ref( (this->*Ator_wv)(wi,v) ));
		endwalk

		if(z.size()>0){										// if there are vals left, these are appearing only in the current box/box/row/col in the specified row/col/box/box
			int n = (this->*Ntor_wv)(wdim(wh2,0),v);		// remove remaining vals from the rest of the box/box/row/col
			walk(mi,mdim) except( (this->*Vtor_nm)(n,mi)==v )
				if( rem( ref( (this->*Ator_nm)(n,mi) ), z )
								&& post_change((this->*Ator_nm)(n,mi))==evt::invalid ){
					post_error( (this->*Ator_nm)(n,mi) );
					return 0;
				}
			endwalk
		}
	endwalk
	return 1;
}
*/






/*
	sudSolve - implementation
*/

	/// construction

sudSolve::sudSolve(const sudSudoku*sd)
	:m_riddle(sd)
{
	m_dimx = m_dimy =0;
	m_processing = 0;
	setproc(proc_simple|proc_smart|proc_clever|proc_assump);
}

sudSolve::~sudSolve(){
	delete m_processing; }

void sudSolve::refresh(int nmaxl){
	prepare(nmaxl);
	getchild()->refresh( nmaxl ); }
void sudSolve::test(int nmaxl){
	prepare(nmaxl);
	getchild()->test(nmaxl); }


void sudSolve::prepare(int nmaxl){
	m_maxlev = nmaxl;

	if(m_dimx==getriddle()->b_x() && m_dimy==getriddle()->b_y())
		m_processing->setmaxlevel(nmaxl);
	else{
		delete m_processing;
		m_processing = 0;

		m_dimx = getriddle()->b_x();
		m_dimy = getriddle()->b_y();

		switch(m_dimx){		/* try to get the faster class working! but avoid to much compile time and exe size */
/*		case 1:
			switch(m_dimy){
				case 1: m_processing = new sudStatProcessing<1,1>(getriddle(),nmaxl); break;
				case 2: m_processing = new sudStatProcessing<1,2>(getriddle(),nmaxl); break;
				case 3: m_processing = new sudStatProcessing<1,3>(getriddle(),nmaxl); break;
				case 4: m_processing = new sudStatProcessing<1,4>(getriddle(),nmaxl); break;
				case 5: m_processing = new sudStatProcessing<1,5>(getriddle(),nmaxl); break;
				case 6: m_processing = new sudStatProcessing<1,6>(getriddle(),nmaxl); break;
			};
			break;*/
		case 2:
			switch(m_dimy){
//				case 1: m_processing = new sudStatProcessing<2,1>(getriddle(),nmaxl); break;
//				case 2: m_processing = new sudStatProcessing<2,2>(getriddle(),nmaxl); break;
				case 3: m_processing = new sudStatProcessing<2,3>(getriddle(),nmaxl); break;
				case 4: m_processing = new sudStatProcessing<2,4>(getriddle(),nmaxl); break;
				case 5: m_processing = new sudStatProcessing<2,5>(getriddle(),nmaxl); break;
//				case 6: m_processing = new sudStatProcessing<2,6>(getriddle(),nmaxl); break;
			};
			break;
		case 3:
			switch(m_dimy){
//				case 1: m_processing = new sudStatProcessing<3,1>(getriddle(),nmaxl); break;
				case 2: m_processing = new sudStatProcessing<3,2>(getriddle(),nmaxl); break;
				case 3: m_processing = new sudStatProcessing<3,3>(getriddle(),nmaxl); break;
				case 4: m_processing = new sudStatProcessing<3,4>(getriddle(),nmaxl); break;
//				case 5: m_processing = new sudStatProcessing<3,5>(getriddle(),nmaxl); break;
//				case 6: m_processing = new sudStatProcessing<3,6>(getriddle(),nmaxl); break;
			};
			break;
		case 4:
			switch(m_dimy){
//				case 1: m_processing = new sudStatProcessing<4,1>(getriddle(),nmaxl); break;
				case 2: m_processing = new sudStatProcessing<4,2>(getriddle(),nmaxl); break;
				case 3: m_processing = new sudStatProcessing<4,3>(getriddle(),nmaxl); break;
				case 4: m_processing = new sudStatProcessing<4,4>(getriddle(),nmaxl); break;
//				case 5: m_processing = new sudStatProcessing<4,5>(getriddle(),nmaxl); break;
//				case 6: m_processing = new sudStatProcessing<4,6>(getriddle(),nmaxl); break;
			};
			break;
		case 5:
			switch(m_dimy){
//				case 1: m_processing = new sudStatProcessing<5,1>(getriddle(),nmaxl); break;
				case 2: m_processing = new sudStatProcessing<5,2>(getriddle(),nmaxl); break;
//				case 3: m_processing = new sudStatProcessing<5,3>(getriddle(),nmaxl); break;
//				case 4: m_processing = new sudStatProcessing<5,4>(getriddle(),nmaxl); break;
//				case 5: m_processing = new sudStatProcessing<5,5>(getriddle(),nmaxl); break;
//				case 6: m_processing = new sudStatProcessing<5,6>(getriddle(),nmaxl); break;
			};
			break;
/*		case 6:
			switch(m_dimy){
				case 1: m_processing = new sudStatProcessing<6,1>(getriddle(),nmaxl); break;
				case 2: m_processing = new sudStatProcessing<6,2>(getriddle(),nmaxl); break;
				case 3: m_processing = new sudStatProcessing<6,3>(getriddle(),nmaxl); break;
				case 4: m_processing = new sudStatProcessing<6,4>(getriddle(),nmaxl); break;
				case 5: m_processing = new sudStatProcessing<6,5>(getriddle(),nmaxl); break;
				case 6: m_processing = new sudStatProcessing<6,6>(getriddle(),nmaxl); break;
			};
			break;*/
		}
		if(!m_processing)
			m_processing = new sudDynProcessing(getriddle(),nmaxl);

	}

	if(m_proc!= getchild()->getproc())
		getchild()->setproc(m_proc);

	getchild()->setriddle(getriddle());
}