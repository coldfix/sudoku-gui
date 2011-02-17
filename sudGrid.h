/////////////////////////////////////////////////////////////////////////////
// Name:        sudgrid.h
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     09/07/2007 22:10:16
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 09/07/2007 22:10:16

#ifndef _SUDGRID_H_
#define _SUDGRID_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sudgrid.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/grid.h"
////@end includes
#include "Sudoku.h"
#include "sudEditor.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class sudGrid;
////@end forward declarations
class sudGridTable;
class sudGridCellEditor;
class sudEditor;


/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SUDGRID_STYLE 0
#define SYMBOL_SUDGRID_IDNAME ID_GRID
#define SYMBOL_SUDGRID_SIZE wxDefaultSize
#define SYMBOL_SUDGRID_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * sudGrid class declaration
 */

class sudGrid: public wxGrid
{    
    DECLARE_DYNAMIC_CLASS( sudGrid )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    sudGrid();
    sudGrid(wxWindow* parent, wxWindowID id = ID_GRID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);

	void setdisplaysize(int size);
	int getdisplaysize() const { return m_displaysize; }

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_GRID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);

    /// Destructor
    ~sudGrid();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();
    bool CreateGrid(int nbx,int nby,wxGrid::wxGridSelectionModes selmode=wxGrid::wxGridSelectCells);

	/// Grid pens
    wxPen GetRowGridLinePen(int row);
    wxPen GetColGridLinePen(int col);

	/// update
	void UpdateDimensions();

	/// sudoku
	void NewGrid(int nbx=0,int nby=0);
    void InitFromSudoku(const Sudoku&);

	bool SetCellVal(int x,int y, int val);
	int GetCellVal(int x,int y) ;

	bool IsReadOnly(int row,int col) const;

////@begin sudGrid event handler declarations

////@end sudGrid event handler declarations

	sudEditor* GetEditor()				{ return m_editor; }
	const sudEditor* GetEditor() const	{ return m_editor; }
	const sudSize* GetSudokuSize() const;

////@begin sudGrid member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end sudGrid member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin sudGrid member variables
    /// Control identifiers
    enum {
        ID_GRID = 10003
    };
////@end sudGrid member variables

	sudEditor* m_editor;
	int m_displaysize;
};


/*
	sudGridCellEditor
*/

// the editor for numeric (long) data
class sudGridCellEditor : public wxGridCellTextEditor
{
public:
    // allows to specify the range - if min == max == -1, no range checking is
    // done
    sudGridCellEditor( );

    void Create(wxWindow* parent,
                        wxWindowID id,
                        wxEvtHandler* evtHandler);

    bool IsAcceptedKey(wxKeyEvent& event);
    void BeginEdit(int row, int col, wxGrid* grid);
    bool EndEdit(int row, int col, wxGrid* grid);

    void Reset();
    void StartingKey(wxKeyEvent& event);

    // parameters string format is "min,max"
    void SetParameters(const wxString& params);

    wxGridCellEditor *Clone() const
        { return new sudGridCellEditor( ); }

    // added GetValue so we can get the value which is in the control
    wxString GetValue() const;

protected:

    // string representation of m_valueOld
    wxString GetString() const
        { return wxString::Format(_T("%ld"), m_valueOld); }

private:
    long m_valueOld;

    DECLARE_NO_COPY_CLASS(sudGridCellEditor)
};


/*
	sudGridTable
*/


class sudGridTable : public wxGridTableBase, public sudEditor
{
public:
    sudGridTable( int _nbx, int _nby );
    ~sudGridTable();

	// internal:

	void NotifyUpdateSize();
	void NotifyUpdateCell(int row,int col);

	// sudEditor

	bool SetSize(int nbx,int nby);
	bool EnterInitMode(bool on);
	bool SetCell(int x, int y, int val);
	void SetShowSolution(bool on);
	void RefreshProcAll();


    // these are pure virtual in wxGridTableBase
	int GetNumberRows()										{ return GetSize()->high(); }
    int GetNumberCols()										{ return GetSize()->high(); }
	wxString GetValue( int row, int col );
	void SetValue( int row, int col, const wxString& s );
	bool IsEmptyCell( int row, int col )					{ return GetValueAsLong(row,col)==0; }

    // Data type determination and value access
    wxString GetTypeName( int row, int col );
	bool CanGetValueAs( int row, int col, const wxString& typeName )	{return typeName == wxGRID_VALUE_NUMBER || typeName==wxGRID_VALUE_STRING;}
	bool CanSetValueAs( int row, int col, const wxString& typeName )	{return typeName == wxGRID_VALUE_NUMBER || typeName==wxGRID_VALUE_STRING;}

	long GetValueAsLong( int row, int col )					{ return GetCell(col,row); }
	void SetValueAsLong( int row, int col, long value )		{ SetCell(col,row,value); }


	void Clear()											{ ClearSudoku(); }

	//
    virtual wxGridCellAttr *GetAttr( int row, int col, wxGridCellAttr::wxAttrKind  kind );



// atm we dont support changing the sudoku size by inserting/appending/deleting rows/cols
//	set the sudoku size directly instead
	bool InsertRows( size_t pos = 0, size_t numRows = 1 )	{ return false; }
    bool AppendRows( size_t numRows = 1 )					{ return false; }
    bool DeleteRows( size_t pos = 0, size_t numRows = 1 )	{ return false; }
    bool InsertCols( size_t pos = 0, size_t numCols = 1 )	{ return false; }
    bool AppendCols( size_t numCols = 1 )					{ return false; }
    bool DeleteCols( size_t pos = 0, size_t numCols = 1 )	{ return false; }


    // Does this table allow attributes?  creates a sudGridCellAttrProvider if necessary.
	bool CanHaveAttributes()								{ return GetAttrProvider(); }

	wxGridCellAttr* GetModeAttr(int mode);
private:
	wxGridCellAttr *m_attrInitial,*m_attrSolved,*m_attrFalseInput,*m_attrUser,*m_attrInvalid;

    DECLARE_CLASS( sudGridTable )
};


typedef sudGridTable sudEdit;	// the alternative name gives a good hint to the table's intention and functionality


#endif
    // _SUDGRID_H_
