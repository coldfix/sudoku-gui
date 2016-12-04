#ifndef __MSUDOKU_H__INCLUDED__
#define __MSUDOKU_H__INCLUDED__

#include "Sudoku.h"
#include <vector>

class mSudoku
    : public Sudoku
{
    typedef unsigned char flagtype;
    std::vector<flagtype> m_flags;
public:

    enum
    {
        mode_riddle,
        mode_solution,
        mode_user,
        mode_incorrect,
        mode_mask = 0x0f,
        flag_invalid = 0x10,
    };

    mSudoku()
    {
    }

    mSudoku(const Sudoku& sudoku)
        : Sudoku(sudoku),
          m_flags(sudoku.size(), mode_riddle)
    {
    }

    mSudoku(const mSudoku& sudoku)
        : Sudoku(sudoku),
          m_flags(sudoku.m_flags)
    {
    }

    mSudoku(int nbx, int nby)
    {
        setsize(nbx, nby);
    }

    mSudoku& operator = (const Sudoku& sudoku)
    {
        Sudoku::operator = (sudoku);
        m_flags.clear();
        m_flags.resize(size(), mode_riddle);
        return *this;
    }

    mSudoku& operator = (const mSudoku& sudoku)
    {
        Sudoku::operator = (sudoku);
        m_flags = sudoku.m_flags;
        return *this;
    }

    void merge(const Sudoku& sudoku, int flg)
    {
        if (sudoku.bx() != bx() || sudoku.by() != by())
            setsize(sudoku.bx(), sudoku.by());

        for (int a = 0; a < size(); a++)
        {
            if (sudoku[a])
            {
                set(a, sudoku[a]);
                setflags(a, flg);
            }
        }
    }

    Sudoku filter(int mode, int mask = mode_mask) const
    {
        Sudoku sudoku(bx(), by());

        for (int a = 0; a < size(); a++)
        {
            if ((getflags(a) & mode_mask) == mode)
            {
                sudoku.set(a, get(a));
            }
        }
        return sudoku;
    }

    void clear()
    {
        Sudoku::clear();
        std::fill(m_flags.begin(), m_flags.end(), mode_riddle);
    }

    void setsize(int nbx, int nby)
    {
        Sudoku::setsize(nbx, nby);
        m_flags.resize(size(), mode_riddle);
    }

    int pt(int x, int y) const { return a_xy(x, y); }

    int getflags(int a) const { return m_flags[a]; }
    int getmode(int a) const { return getflags(a) & mode_mask; }

    bool isuser(int a) const { return isusercorrect(a) || isuserincorrect(a); }
    bool isusercorrect(int a) const { return getmode(a) == mode_user; }
    bool isuserincorrect(int a) const { return getmode(a) == mode_incorrect; }
    bool isriddle(int a) const { return getmode(a) == mode_riddle; }
    bool issolution(int a) const { return getmode(a) == mode_solution; }

    bool isinvalid(int a) const { return getflags(a) & flag_invalid; }

    void setflags(int a, int flags) { m_flags[a] = flags; }
    void setmode(int a, int mode) { m_flags[a] = m_flags[a] & ~mode_mask | mode; }

    void setriddle(int a) { setmode(a, mode_riddle); }
    void setsolution(int a) { setmode(a, mode_solution); }
    void setuser(int a) { setmode(a, mode_user); }
    void setincorrect(int a) { setmode(a, mode_incorrect); }

    void setinvalid(int a) { setflags(a, getflags(a) | flag_invalid); }
    void setvalid(int a) { setflags(a, getflags(a) & ~flag_invalid); }

};


#endif // __MSUDOKU_H__INCLUDED__

