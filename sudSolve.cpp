#include <wx/wxprec.h>


#include "sudSolve.h"

#include <cstdlib>
#include <string.h>     // memcpy, memset



int getrand(int range, int min){
    return std::rand()/2%range + min; }

int getrand(int range){
    return std::rand()/2%range; }



/*
*/

sudSolve::sudSolve()
{
}

sudSolve::sudSolve(int nbx,int nby)
{
    resize(nbx, nby, true);
}

sudSolve::sudSolve(const Sudoku& sudoku)
{
    init(sudoku);
}

sudSolve::~sudSolve()
{
    if (initialized()){
        delete [] m_states;
        delete [] m_possible; }
}


/*
*/

void sudSolve::resize(int _nbx, int _nby, bool clear)
{
    bool realloc = _nbx * _nby != high();

    if (initialized() && realloc){
        delete [] m_states;
        delete [] m_possible; }

    sudSize::init(_nbx, _nby);

    if (initialized()){
        if (realloc){
            m_states = new signed char[size()];
            m_possible = new bool[datasize()]; }
        if (clear){
            memset(m_possible, 1, datasize());
            memset(m_states, (char)(-high()), size());
        }
    }

    m_error = !initialized();
    m_immediate = false;
    m_unsolved = size();
    m_dirty = true;
}

void sudSolve::setsize(int _nbx,int _nby){
    resize(_nbx, _nby, true); }


void sudSolve::init(const Sudoku& sudoku){
    resize(sudoku.bx(), sudoku.by(), true);

    for (int a = 0; a < size(); a++)
        if (sudoku.get(a))
            setsolution(a,sudoku.get(a));
}



/*
*/


Sudoku sudSolve::sudoku() const
{
    Sudoku sudoku(bx(), by());
    for (int a = 0; a < size(); a++)
        if (solved(a))
            sudoku.set(a, solution(a));
    return sudoku;
}

void sudSolve::getpossible(int a, valarray& va) const
{
    va.clear();
    va.reserve(high());
    for (int v = 1; v <= high(); v++)
        if (possible(a,v))
            va.push_back(v);
}

int sudSolve::possibility(int a,int i) const
{
    for (int v = 1; v <= high(); v++)
        if (possible(a,v) && ! i--)
            return v;
    return 0;
}




/*
*/


void sudSolve::setsolution(int a,int val)
{
    if (solved(a))
        return;
    --m_unsolved;
    m_dirty = true;

    m_states[a] = val;                              // update field info
    for (int v = 1; v <= high(); v++)
        if (v != val)
            valfield(v)[a] = false;

    bool * vpossible = valfield(val);
    if (!vpossible[a])
        m_error = true;

    if (error() && m_immediate /*|| solved()&&!error()*/)       // .. show all invalid cells in case of failure
        return;


    int y = y_a(a),
        x = x_a(a);

    for (int xi = 0; xi < high(); xi++)                 // walk row
        if (vpossible[a_xy(xi, y)] && xi != x){
            discard(a_xy(xi, y), val);
            if (error() && m_immediate)
                return; }

    for (int yi = 0; yi < high(); yi++)                 // walk col
        if (vpossible[a_xy(x, yi)] && yi != y){
            discard(a_xy(x,yi), val);
            if (error() && m_immediate)
                return; }

    int b = b_xy(x,y),
        f = f_xy(x,y);

    for (int fi = 0; fi < high(); fi++)                 // walk box
        if (vpossible[a_bf(b, fi)] && fi != f){
            discard(a_bf(b, fi), val);
            if (error() && m_immediate)
                return; }
}


void sudSolve::discard(int a, int val)
{
    if (!possible(a,val))
        return;
    valfield(val)[a] = false;
    m_dirty = true;

    if (solution(a) == val){
        m_error = true;
        return; }
    if (solved(a))
        return;

    ++ m_states[a];
    if (possibilities(a) == 1){
        for (int v = 1; v <= high(); v++)
            if (possible(a,v)){
                setsolution(a,v);
                break; }
    }
}


void sudSolve::deepthoughts(bool smart,bool clever)
{
    while (dirty()){
        if (m_error && m_immediate || solved())
            break;
        m_dirty = false;
        if (smart){
            thinksmart();
            m_smart = true; }
        if (dirty())
            continue;
        if (clever){
            thinkclever();
            m_clever = true; }
    }
}

/*

    thinksmart algorithm:

    walk through unit U
    if C is the only cell of U that can possibly be solved by the value V
    then solve cell C with the value V

*/

void sudSolve::thinksmart()
{
    for (int a = 0; a < size() && !dirty(); a++){           // search for unique values
        if (solved(a))
            continue;
        int x = x_a(a), y = y_a(a),
            b = b_a(a), f = f_a(a);

        for (int v = 1; v <= high(); v++){
            bool* possible = valfield(v);
            if (!possible[a])
                continue;

            bool solve = true;
            for (int xi = 0; xi < high(); xi++)             // in one row
                if (possible[a_xy(xi, y)] && xi != x){
                    solve = false;
                    break; }
            if (solve){
                setsolution(a, v);
                break; }

            solve = true;
            for (int yi = 0; yi < high(); yi++)             // in one column
                if (possible[a_xy(x, yi)] && yi != y){
                    solve = false;
                    break; }
            if (solve){
                setsolution(a, v);
                break; }

            solve = true;
            for (int fi = 0; fi < high(); fi++)             // in one box
                if (possible[a_bf(b, fi)] && fi != f){
                    solve = false;
                    break; }
            if (solve){
                setsolution(a, v);
                break; }
        }
    }
    if (dirty()){
        // .. debug count.. ?
    }
}


/*

    thinkclever algorithm

    walk through unit U.
    if there is a subunit SU which can hold a value V as the only subunit of U,
    then discard V in the accurate "cross-unit" CU of SU

    CU is (besides U) the only unit that completely includes SU
*/

void sudSolve::thinkclever()
{
    for (int v = 1; v <= high(); v++){      //search vals that are possible in only
        bool *possible = valfield(v);

        for (int x = 0; x < high(); x++){           // one box's col within a col
            bool operate = false;
            int yb;
            for (int y = 0; y < high(); y++)
                if (possible[a_xy(x,y)]){
                    operate = !solved(a_xy(x,y));
                    yb = yb_y(y);
                    break; }
            if (!operate)
                continue;
            for (int y = y_hl(yb+1, 0); y < high(); y++)
                if (possible[a_xy(x, y)]){
                    operate = false;
                    break; }
            if (!operate)
                continue;
            int b = b_hl(xb_x(x), yb),                  // and remove this possibility from the rest of the box
                xf = xf_x(x);
            for (int f = 0; f < high(); f++)
                if (xf_f(f) != xf && possible[a_bf(b,f)]){
                    discard(a_bf(b,f), v);
                    m_dirty = true; }
            if (dirty()){
                return; }
        }

        for (int y = 0; y < high(); y++){           // in one box's row of a row
            bool operate = false;
            int xb;
            for (int x = 0; x < high(); x++)
                if (possible[a_xy(x,y)]){
                    operate = !solved(a_xy(x,y));
                    xb = xb_x(x);
                    break; }
            if (!operate)
                continue;
            for (int x = x_hl(xb+1, 0); x < high(); x++)
                if (possible[a_xy(x,y)]){
                    operate = false;
                    break; }
            if (!operate)
                continue;
            int b = b_hl(xb, yb_y(y)),                  // and remove this possibility from the rest of the box
                yf = yf_y(y);
            for (int f = 0; f < high(); f++)
                if (yf_f(f) != yf && possible[a_bf(b,f)]){
                    discard(a_bf(b,f), v);
                    m_dirty = true; }
            if (dirty())
                return;
        }

        for (int b = 0; b < high(); b++){               // in one col of a box
            bool operate = false;
            int xf;
            for (int f = 0; f < high(); f++)
                if (possible[a_bf(b, f)]){
                    operate = !solved(a_bf(b, f));
                    xf = xf_f(f);
                    break; }
            if (!operate)
                continue;
            for (int f = f_hl(xf+1, 0); f < high(); f++)
                if (possible[a_bf(b, f)]){
                    operate = false;
                    break; }
            if (!operate)
                continue;
            int x=x_hl(xb_b(b),xf),                         // and remove these vals in the rest of the col
                yb=yb_b(b);
            for (int y = 0; y < high(); y++)
                if (yb_y(y) != yb && possible[a_xy(x,y)]){
                    discard(a_xy(x,y), v);
                    m_dirty = true; }
            if (dirty())
                return;
        }

        for (int b = 0; b < high(); b++){               // in one row of a box
            bool operate = false;
            int yf;
            for (int f = 0; f < high(); f++)
                if (possible[a_bf(b, f)]){
                    operate = !solved(a_bf(b, f));
                    yf = yf_f(f);
                    break; }
            if (!operate)
                continue;
            for (int f = f_hl(0, yf+1); f < high(); f++)
                if (possible[a_bf(b,f)] && yf_f(f) != yf){
                    operate = false;
                    break; }
            if (!operate)
                continue;
            int y = y_hl(yb_b(b), yf),                      // and remove these vals in the rest of the row
                xb = xb_b(b);
            for (int x = 0; x < high(); x++)
                if (xb_x(x) != xb && possible[a_xy(x,y)]){
                    discard(a_xy(x,y), v);
                    m_dirty = true; }
            if (dirty())
                return;
        }
    }
}



/*

*/




/// solve

void sudSolve::solve(int maxlev,Yielder*yielder){
    solve_(method_findunique,maxlev,yielder); }

void sudSolve::fill(Yielder*yielder){
    solve_(method_findany,-1,yielder); }

void sudSolve::test(int maxlev,Yielder*yielder){
    solve_(method_test,maxlev,yielder); }


void sudSolve::solve_(int method,int maxlevels,Yielder*yielder)
{
    bool smart =true, clever = false;

    m_smart = false;
    m_clever = false;
    m_immediate = method == method_findany||method == method_test;
    m_iusedlevel = 0;
    deepthoughts(smart,clever);

    Sudoku state = sudoku(), solution;

    struct level{
        explicit level(int _a) : a(_a)  {}
        int a;
        valarray vals;
    };

    std::vector<level> levelstack;
    int currentlevel=0;
    bool found = solved() && !error(),
        toohard = false, multiple = false, usesolution = false,
        pushlevel = !solved() && !error(),
        userand = method == method_findany;

    while (true){
        if (pushlevel){
            if (currentlevel == maxlevels){
                toohard = true;
                break; }
            int a = 0, cardinality = high()+1;
            for (int ai = 0; ai < size(); ai++)
                if (!solved(ai) && possibilities(ai) < cardinality){
                    a = ai;
                    cardinality = possibilities(a); }
            levelstack.push_back(level(a));
            getpossible(a, levelstack.back().vals);
            if (++currentlevel > m_iusedlevel)
                m_iusedlevel = currentlevel;
            if (yielder && !yielder->yield())               // yield !
                return;
        }

        if (levelstack.empty())
            break;

        level& l = levelstack.back();
        if (l.vals.empty()){                    // pop level
            levelstack.pop_back();
            --currentlevel;
            if (!levelstack.empty())
                state.set(levelstack.back().a, 0);
            init(state);
            m_immediate = method == method_findany||method == method_test;
            pushlevel = false;
        }else{
            int iv = userand ? getrand(l.vals.size()) : l.vals.size()-1;
            int v = l.vals[iv];
            l.vals.erase(l.vals.begin()+iv);
            setsolution(l.a, v);
            deepthoughts(smart,clever);

            if (solved() && !error()){
                if (found && (method == method_findunique||method == method_test)){
                    multiple = true;
                    break;
                }else{
                    found = true;
                    if (method == method_findany)
                        break;
                    solution = sudoku();
                    usesolution = true;
                }
            }

            if (! error() && !solved()){
                state.set(l.a, v);
                pushlevel = true;
            }else{
                if (!l.vals.empty())
                    init(state);
                pushlevel = false; }
        }
    }

    if (multiple || toohard)
    {
        if (m_iusedlevel){
            for (int i = 0; i < levelstack.size(); i++){
                level& l = levelstack[i];
                state.set(l.a, 0); }
            init(state);
            thinksmart(); }
    }
    else if (found)
    {
        if (usesolution)
            init(solution);
    }
    else        // ... error
    {
        if (m_iusedlevel){
            for (int i = 0; i < levelstack.size(); i++){
                level& l = levelstack[i];
                state.set(l.a, 0); }
            init(state);
            thinksmart(); }
        m_error = true;
    }

    if (!m_smart)
        m_difficulty = difficulty_easy;
    else if (m_iusedlevel == 0 && !m_clever)
        m_difficulty = difficulty_medium;
    else
        m_difficulty = difficulty_hard;

    m_multiple = multiple;
    m_unique = solved() && !error();
    m_toohard = toohard;
    m_hassolution = found;

    m_immediate = false;
}

