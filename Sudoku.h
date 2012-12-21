#ifndef __SUDOKU_H__INCLUDED__
#define __SUDOKU_H__INCLUDED__


typedef unsigned char valtype;



/*
    class sudSize
        > represents the dimensions of a sudoku
        > dimensions can be set at runtime
*/

class sudSize{
public:
    /// construction

    sudSize(void)                   { init(0,0); }
    sudSize(int bx,int by)          { init(bx,by); }
    sudSize(const sudSize&size)     { init(size.bx(),size.by()); }
    virtual ~sudSize() {}

protected:
    void init(int _nbx, int _nby)
    {
        nbx = _nbx;
        nby = _nby;
        row = nbx * nby;
        total = row * row;
    }
public:

    /// size

    virtual void setsize(int bx, int by)
    {
        init(bx,by);
    }

    int bx() const                  { return nbx; }
    int by() const                  { return nby; }
    int fx() const                  { return nby; }
    int fy() const                  { return nbx; }
    int high() const                { return row; }
    int size() const                { return total; }

    bool initialized() const        { return bx() && by(); }

    int maxval() const              { return row; }
    int minval() const              { return 1; }

    bool operator  == (const sudSize& ds) const{
        return bx() == ds.bx() && by() == ds.by(); }


    /// coord transformations

    int a_xy(int x,int y) const     { return x * high() + y; }
    int a_bf(int b,int f) const     { return a_xy(x_bf(b,f), y_bf(b,f)); }

    int x_a(int a) const            { return a / high(); }
    int y_a(int a) const            { return a % high(); }
    int b_a(int a) const            { return b_hl(xb_a(a), yb_a(a)); }
    int f_a(int a) const            { return f_hl(xf_a(a), yf_a(a)); }

    int x_bf(int b,int f) const     { return x_hl(xb_b(b), xf_f(f)); }
    int y_bf(int b,int f) const     { return y_hl(yb_b(b), yf_f(f)); }
    int b_xy(int x,int y) const     { return b_hl(xb_x(x), yb_y(y)); }
    int f_xy(int x,int y) const     { return f_hl(xf_x(x), yf_y(y)); }

    int x_hl(int h,int l) const     { return h * fx() + l; }
    int y_hl(int h,int l) const     { return h * fy() + l; }
    int b_hl(int h,int l) const     { return h * by() + l; }
    int f_hl(int h,int l) const     { return h * fy() + l; }

    int xb_x(int x) const           { return x / fx(); }
    int xf_x(int x) const           { return x % fx(); }
    int yb_y(int y) const           { return y / fy(); }
    int yf_y(int y) const           { return y % fy(); }

    int xb_b(int b) const           { return b / by(); }
    int xf_f(int f) const           { return f / fy(); }
    int yb_b(int b) const           { return b % by(); }
    int yf_f(int f) const           { return f % fy(); }

    int xb_a(int a) const           { return xb_x(x_a(a)); }
    int xf_a(int a) const           { return xf_x(x_a(a)); }
    int yb_a(int a) const           { return yb_y(y_a(a)); }
    int yf_a(int a) const           { return yf_y(y_a(a)); }

protected:

    /// members

    int nbx, nby, row, total;
};



/*
    class Sudoku
        > represents a sudoku with n_box_x * n_box_y boxes with each n_box_y*n_box_x fields
        > size can be set at runtime

    we dont provide a statSudoku class because the sudoku class isn't used in performance-sensitive context
*/

class Sudoku : public sudSize{
public:

    /// construction & destruction

    Sudoku()
    {
        m_data = 0;
        m_unsolved = 0;
    }

    Sudoku(int _nbx, int _nby,
           const valtype* data = 0)
    {
        m_data = 0;
        init(_nbx, _nby, data);
    }

    Sudoku(const Sudoku& sd)
        : sudSize(sd)
    {
        ref(sd.m_data);
        m_unsolved = sd.m_unsolved;
    }

    ~Sudoku(void)
    {
        free(m_data);
    }


    /// (re-)initialization

    virtual void clear()
    {
        init(bx(), by());
    }

protected:
    void init(int _nbx, int _nby,
               const valtype* data = 0)
    {
        int h = high();
        sudSize::setsize(_nbx, _nby);

        if (_nbx * _nby != h || shared())
            unshare(false);

        if (data)
        {
            m_unsolved = size();
            for (int a = 0; a < size(); a++)
                if (m_data[a] = data[a])
                    --m_unsolved;
        }
        else
        {
            for (int a = 0; a < size(); a++)
                m_data[a] = 0;
            m_unsolved = size();
        }
    }

    bool shared() const { return m_data && m_data[-1] != 0; }
    void unshare(bool copy = true)
    {
        valtype* old = m_data;
        m_data = alloc(size());

        if (copy && m_data && old)
        {
            for (int i = 0; i < size(); i++)
                m_data[i] = old[i];
        }
        free(old);
    }

    static valtype* alloc(int size)
    {
        if (size == 0)
            return 0;
        valtype *data = 1 + new valtype[size + 1];
        data [-1] = 0;
        return data;
    }
    static void free(valtype * data)
    {
        if (data && data[-1]-- == 0)
            delete [] (data - 1);
        data = 0;
    }
    void ref(valtype * data)
    {
        m_data = data;
        ++m_data[-1];
    }
public:

    void setsize(int _nbx, int _nby)
    {
        init(_nbx, _nby);
    }

    Sudoku& operator = (const Sudoku& sudoku)
    {
        free(m_data);
        sudSize::operator = (sudoku);
        ref(sudoku.m_data);
        m_unsolved = sudoku.m_unsolved;
        return *this;
    }


    /// test

    int getunsolved() const { return m_unsolved; }
    int getsolved() const { return size() - getunsolved(); }
    bool empty() const { return getsolved() == 0; }
    bool filled() const { return getunsolved() == 0; }
    bool solved() const { return filled() && valid(); }

    bool equals(const Sudoku& sud) const{
        if (bx() != sud.bx() || by() != sud.by())
            return false;
        for (int a = 0; a < size(); a++)
            if (get(a) != sud.get(a))
                return false;
        return true; }

    bool operator == (const Sudoku&sud) const{
        return equals(sud); }

    bool valid() const
    {
        for (int a = 0; a < size(); a++)
        {
            int x = x_a(a), y = y_a(a),
                b = b_a(a), f = f_a(a),
                v = get(a);
            if (!v)
                continue;
            for (int xi = x + 1; xi < high(); xi++)
                if (get(a_xy(xi, y)) == v)
                    return false;
            for (int yi = y + 1; yi < high(); yi++)
                if (get(a_xy(x, yi)) == v)
                    return false;
            for (int fi = f + 1; fi < high(); fi++)
                if (get(a_bf(b, fi)) == v)
                    return false;
        }
        return true;
    }


    /// coord-access

    valtype get(int a) const{
        return m_data[a]; }

    valtype get(int x,int y) const{
        return get(a_xy(x,y)); }

    valtype operator [] (int a) const{
        return get(a); }


    void set(int a, valtype num)
    {
        if (shared())
            unshare();
        if (num && !m_data[a])
            --m_unsolved;
        else if (!num&&m_data[a])
            ++m_unsolved;
        m_data[a] = num;
    }

    void set(int x, int y, valtype num) {
        set(a_xy(x,y), num); }

private:
    /// members
    valtype* m_data;
    int m_unsolved;
};




/*

*/





#endif // __SUDOKU_H__INCLUDED__
