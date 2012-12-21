#ifndef __SUDOKUFILE_H__INCLUDED__
#define __SUDOKUFILE_H__INCLUDED__

#include "mSudoku.h"
#include <wx/string.h>


class wxOutputStream;
class wxInputStream;


class SudokuSave
{
public:
    SudokuSave();
    ~SudokuSave();

    bool open(const wxString& file);
    bool open(wxOutputStream*, bool del = false);
    void close();

    bool ok() const;

    void save(const mSudoku&);

    virtual wxString file_header();
    virtual wxString file_entry(const mSudoku&);
    virtual wxString file_footer();

protected:
    wxOutputStream *m_stream;
    bool m_delete;
};

class SudokuSaveHtml
    : public SudokuSave
{
public:
    wxString file_header();
    wxString file_entry(const mSudoku&);
    wxString file_footer();
};


class SudokuLoad
{
public:
    SudokuLoad();
    ~SudokuLoad();

    bool open(const wxString& file);
    bool open(wxInputStream*, bool del = false);
    void close();

    bool ok() const;

    virtual bool load(mSudoku&);

    bool eof();
    bool fail();

protected:
    wxInputStream *m_stream;
    bool m_delete;
};


class SudokuLoadHtml
    : public SudokuLoad
{
public:
    bool load(mSudoku&);
};



#endif // __SUDOKUFILE_H__INCLUDED__
