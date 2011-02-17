#include <wx/wxprec.h>

#include "sudokusave.h"

#include <wx/stream.h>		// wxOutputStream
#include <wx/wfstream.h>	// wxFileOutputStream
#include <wx/txtstrm.h>		// wxTextOutputStream
#include <wx/sstream.h>		// wxStringOutputStream

#include <locale>


/*
*/

bool hasborder(int num,int dim,int high){
	return !(num%dim) && num && num != high; }

void append_if(wxString& str, bool condition, const wxString& append)
{
	if (condition)
	{
		if (!str.empty())
			str += ' ';
		str += append;
	}
}


/*
	SudokuSave
*/

SudokuSave::SudokuSave()
{
	m_stream = 0;
}

SudokuSave::~SudokuSave()
{
	close();
}

bool SudokuSave::open(const wxString& file)
{
	return open(new wxFileOutputStream(file), true);
}

bool SudokuSave::open(wxOutputStream* stream, bool del)
{
	close();
	m_stream = stream;
	m_delete = del;

	if (ok())
	{
		wxTextOutputStream strm(*m_stream);
		strm << file_header();
	}

	return ok();
}

bool SudokuSave::ok() const
{
	return m_stream && m_stream->IsOk();
}

void SudokuSave::close()
{
	if (ok())
	{
		wxTextOutputStream strm(*m_stream);
		strm << file_footer();
	}

	if (m_delete)
		delete m_stream;
	m_stream = 0;
}

void SudokuSave::save(const mSudoku& sudoku)
{
	if (!m_stream)
		return;

	wxTextOutputStream strm(*m_stream);
	strm << file_entry(sudoku);
}

wxString SudokuSave::file_header()
{
	return wxEmptyString;
}

wxString SudokuSave::file_entry(const mSudoku& sudoku)
{
	wxString s = wxString::Format("%i %i\n", sudoku.bx(), sudoku.by());

	for (int y = 0; y < sudoku.high(); y++)
	{
		if (y)
			s += y % sudoku.fy() ? wxT("\n") : wxT("\n\n");

		for (int x = 0; x < sudoku.high(); x++)
		{
			if (x)
				s += (x % sudoku.fx() ? wxT(" ") : wxT("  "));

			int pt = sudoku.pt(x, y);
			if (sudoku.isusercorrect(pt))
				s += '.';

			int val;
			if (sudoku.isusercorrect(pt) || sudoku.isriddle(pt))
				val = sudoku.get(pt);
			else
				val = 0;

			s += wxString::Format("%i", val);
		}
	}

	return s;
}

wxString SudokuSave::file_footer()
{
	return wxEmptyString;
}




/*
*/

#include"html_templates.h"

wxString SudokuSaveHtml::file_header()
{
	return g_TplFileHeaderTxt;
}

wxString SudokuSaveHtml::file_footer()
{
	return g_TplFileFooterTxt;
}

wxString SudokuSaveHtml::file_entry(const mSudoku& sudoku)
{
	int fx = sudoku.fx(),
		fy = sudoku.fy(),
		h = fx * fy;

	wxString file = g_TplFileEntryTxt;
	wxString rows;

	file.Replace(wxT("%SAVE%"), SudokuSave::file_entry(sudoku));


	for (int y = 0; y < h; y++)
	{
		wxString cells, rowclass, row = g_TplRowTxt;

		append_if(rowclass, hasborder(y, fy, h), wxT("tb"));
		append_if(rowclass, hasborder(y+1, fy, h), wxT("bb"));

		for (int x = 0; x < h; x++)
		{
			int pt = sudoku.pt(x, y);
			wxString cell = g_TplCellTxt;

			{ wxString number;
			if (sudoku.get(pt))
				number.Printf(wxT("%ld"), sudoku.get(pt));
			cell.Replace(wxT("%NUMBER%"), number); }

			wxString cellclass;
			switch (sudoku.getmode(sudoku.pt(x,y)))
			{
				case mSudoku::mode_riddle:
					cellclass = wxT("r");
					break;
				case mSudoku::mode_user:
					cellclass = wxT("u");
					break;
				case mSudoku::mode_incorrect:
					cellclass = wxT("incorrect");
					break;
				case mSudoku::mode_solution:
					cellclass = wxT("solution");
					break;
			}

			append_if(cellclass, sudoku.isinvalid(pt), "inv");

			append_if(cellclass, !rowclass.empty(), rowclass);
			append_if(cellclass, hasborder(x, fx, h), wxT("lb"));
			append_if(cellclass, hasborder(x+1, fx, h), wxT("rb"));

			cell.Replace(wxT("%CLASS%"), cellclass);
			cells += cell;
		}

		row.Replace(wxT("%CLASS%"), wxEmptyString);
		row.Replace(wxT("%CELLS%"), cells);
		rows += row;
	}

	file.Replace(wxT("%ROWS%"), rows);

	return file;
}



/*
	SudokuLoad
*/



SudokuLoad::SudokuLoad()
{
	m_stream = 0;
}

SudokuLoad::~SudokuLoad()
{
	close();
}

bool SudokuLoad::open(const wxString& file)
{
	return open(new wxFileInputStream(file), true);
}

bool SudokuLoad::open(wxInputStream* stream, bool del)
{
	close();
	m_stream = stream;
	m_delete = del;
	return ok();
}

bool SudokuLoad::ok() const
{
	return m_stream && m_stream->IsOk();
}

void SudokuLoad::close()
{
	if (m_delete)
		delete m_stream;
	m_stream = 0;
}

bool SudokuLoad::load(mSudoku& sudoku)
{
	if (!m_stream)
		return false;

	wxTextInputStream strm(*m_stream);

	wxInt32 xcol = -1, ycol = -1;
	strm >> xcol >> ycol;
	if (xcol < 1 || ycol < 1)
		return false;

	sudoku . setsize(xcol, ycol);

	for (int y = 0; y < sudoku.high(); y++)
	{
		for (int x = 0; x < sudoku.high(); x++)
		{
			wxChar c;
			do{
				strm >> c;
			} while (std::isspace(c, std::locale()));

			bool user = c == '.';
			if (!user)
				m_stream->Ungetch(c);

			wxInt32 cur = -1;
			strm >> cur;
			if (cur < 0 || cur > sudoku.high())
				return false;

			sudoku . set(x, y, cur);
			if (user)
				sudoku . setuser(sudoku.pt(x, y));
		}
	}

	return true;
}




bool SudokuLoadHtml::load(mSudoku& sudoku)
{
	if (!m_stream)
		return false;

	wxTextInputStream strm(*m_stream);

	while (true)
	{
		if (strm.ReadLine().Contains("<sudoku"))
			break;
		if (m_stream -> Eof())
			return false;
	}

	while (!strm.ReadLine().Contains("<content>"))
	{
		if (m_stream -> Eof())
			return false;
	}

	return SudokuLoad::load(sudoku);
}


