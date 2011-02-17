/////////////////////////////////////////////////////////////////////////////
// Name:        sudgrid.cpp
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     09/07/2007 22:10:16
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 09/07/2007 22:10:16

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sudgrid.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "sudGrid.h"
#include "sudEditor.h"

////@begin XPM images
////@end XPM images


/*!
 * sudGrid type definition
 */

IMPLEMENT_DYNAMIC_CLASS( sudGrid, wxGrid )


/*!
 * sudGrid event table definition
 */

BEGIN_EVENT_TABLE( sudGrid, wxGrid )

////@begin sudGrid event table entries
////@end sudGrid event table entries

END_EVENT_TABLE()


/*!
 * sudGrid constructors
 */

sudGrid::sudGrid()
{
    Init();
}

sudGrid::sudGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)

// calling wxGrid::Create(..) causes program crash, so we use the constructor
: wxGrid(parent,id,pos,size,style) 
{

    Init();
	CreateControls();
//    Create(parent, id, pos, size, style);
}


/*!
 * sudgrid creator
 */

bool sudGrid::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin sudGrid creation
    wxGrid::Create(parent, id, pos, size, style);
    CreateControls();
////@end sudGrid creation
    return true;
}


/*!
 * sudGrid destructor
 */

sudGrid::~sudGrid()
{
////@begin sudGrid destruction
////@end sudGrid destruction
}


/*!
 * Member initialisation
 */

void sudGrid::Init()
{
////@begin sudGrid member initialisation
////@end sudGrid member initialisation
	m_displaysize = -1;
}

void sudGrid::setdisplaysize(int size)
{
	if(m_displaysize == size)
		return;
	m_displaysize = size;
	SetDefaultColSize(35 + 10*(size-1));
	SetDefaultRowSize(35 + 10*(size-1));
	SetFont( wxFont(14 + 3*(size-1),wxDEFAULT, wxNORMAL,wxBOLD) );
	UpdateDimensions();
}

/*!
 * Control creation for sudgrid
 */

void sudGrid::CreateControls()
{    
////@begin sudGrid content construction
    // Generated by DialogBlocks, 17/12/2007 20:33:42 (Personal Edition)

////@end sudGrid content construction
}


bool sudGrid::CreateGrid(int nbx,int nby,wxGrid::wxGridSelectionModes selmode){
	sudGridTable* pgt = new sudGridTable(nbx,nby);
	m_editor = pgt;
	SetTable( pgt, true, selmode);
    SetColLabelSize(0);
    SetRowLabelSize(0);
    
	SetDefaultEditor( new sudGridCellEditor() );
    SetDefaultCellAlignment(wxALIGN_CENTRE,wxALIGN_CENTRE);
    DisableCellEditControl();
    DisableDragColMove();
    DisableDragColSize();
    DisableDragGridSize();
    DisableDragRowSize();
	return true; }


void sudGrid::UpdateDimensions(){
	wxGrid::UpdateDimensions();
	SetSize( GetVirtualSize() );	//Fit();	// because of some reason Fit() shrinks the grid
	GetParent()->GetSizer()->SetSizeHints(GetParent());
	GetParent()->Fit();		// GetParent()->Layout(); /*would not shrink the frame*/
}

wxPen sudGrid::GetRowGridLinePen(int row){
    if(row%GetSudokuSize()->bx() == GetSudokuSize()->bx()-1	// nbx = nfy
							&& row!=GetSudokuSize()->high()-1)
        return wxPen( *wxBLACK, 2, wxSOLID );
    else
        return GetDefaultGridLinePen(); }

wxPen sudGrid::GetColGridLinePen(int col){
    if(col%GetSudokuSize()->by() == GetSudokuSize()->by()-1		// nby = nfx
							&& col!=GetSudokuSize()->high()-1)
        return wxPen( *wxBLACK, 2, wxSOLID );
    else
        return GetDefaultGridLinePen(); }

void sudGrid::NewGrid(int nbx,int nby){
	if( nbx*nby>16 && nbx*nby>GetSudokuSize()->high() )
		setdisplaysize(0);
	else if( nbx*nby<16 && nbx*nby<GetSudokuSize()->high() )
		setdisplaysize(1);
	GetEditor()->SetSize(nbx,nby); }

void sudGrid::InitFromSudoku(const Sudoku&sud){
	NewGrid(sud.bx(),sud.by());
	GetEditor()->SetRiddle(sud);
	GetEditor()->EnterInitMode(0); }

const sudSize* sudGrid::GetSudokuSize() const{
	return GetEditor()->GetSize(); }

/*
	modes
*/


bool sudGrid::IsReadOnly(int row,int col) const{
	return GetEditor()->IsReadOnly(col,row); }

bool sudGrid::SetCellVal(int x,int y, int val){
	if( !GetEditor()->SetCell(x,y,val) )
		return false;
	SendEvent( wxEVT_GRID_CELL_CHANGE, y, x ) ;		//
//	RefreshCell( y,x );		// this is done in editor... but if we're working with edit controls, it does not have to be done.. so don't do it in editor?
	return true; }

int sudGrid::GetCellVal(int x,int y) {
	return GetEditor()->GetCell(x,y); }

/*!
 * Should we show tooltips?
 */

bool sudGrid::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap sudGrid::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin sudGrid bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end sudGrid bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon sudGrid::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin sudGrid icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end sudGrid icon retrieval
}




/*
	todo: improve gridcelltexteditor!!
	> write a n-char edit-control (where n represents the number of necessary digits for the current sudoku)
		- changes cursor-pos when cursor-buttons are pressed: grid->MoveCursorUp/Down/Left/Right
		- when enter is pressed: SetCurrentCell reading style
		- 

	> further improvements: delete/remove key on keyboard shall have effect.. 
*/




// ----------------------------------------------------------------------------
// sudGridCellEditor
// ----------------------------------------------------------------------------

sudGridCellEditor::sudGridCellEditor(){ }

void sudGridCellEditor::Create(wxWindow* parent,
                                    wxWindowID id,
                                    wxEvtHandler* evtHandler)
{
        // just a text control
    wxGridCellTextEditor::Create(parent, id, evtHandler);

    Text()->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
}

void sudGridCellEditor::BeginEdit(int row, int col, wxGrid* grid)
{
	const sudEditor* sedit = ((sudGrid*) grid)->GetEditor();
	m_valueOld = sedit->IsInitMode() ? sedit->GetRiddle(col,row) : sedit->GetUser(col,row);
    DoBeginEdit(GetString());
	
	// todo: break up, if cell is readonly
}

bool sudGridCellEditor::EndEdit(int row, int col,
                                     wxGrid* grid)
{
    long value = 0;
    Text()->GetValue().ToLong(&value);

    if ( value == m_valueOld )
		return false;

	if(value==0){
		grid->GetTable()->SetValueAsLong( row, col, 0 );
		return true; }

	const sudSize* psize = ((sudGrid*)grid)->GetSudokuSize();

	if(value< psize->minval()  || value > psize->maxval())		//
		return 0;

	grid->GetTable()->SetValueAsLong( row, col, value );
	return true; }


void sudGridCellEditor::Reset()
{
    DoReset(GetString());
}

bool sudGridCellEditor::IsAcceptedKey(wxKeyEvent& event)
{
    if ( wxGridCellEditor::IsAcceptedKey(event) )
    {
        int keycode = event.GetKeyCode();
        if ( (keycode < 128) && (wxIsdigit(keycode) ))
        {
			// ... do any specific error control?
            return true;
        }
    }

    return false;
}

void sudGridCellEditor::StartingKey(wxKeyEvent& event)
{
    int keycode = event.GetKeyCode();
    if ( wxIsdigit(keycode) )	// do any further error control?
        wxGridCellTextEditor::StartingKey(event);

	else if( keycode==' '){

		// todo: set field to 0 and change current cell

	}else if(keycode==WXK_DELETE || keycode==WXK_BACK ){
		// todo: set field to 0 and stay
	}if( keycode==WXK_RETURN || keycode==WXK_TAB ){
		// todo: change current cell
	}else
		event.Skip();
}

void sudGridCellEditor::SetParameters(const wxString& params)
{
}

// return the value in the spin control if it is there (the text control otherwise)
wxString sudGridCellEditor::GetValue() const{
    wxString s;
    s = Text()->GetValue();
    return s; }



/*
	sudGridTable
*/

IMPLEMENT_CLASS( sudGridTable, wxGridTableBase )

wxString sudGridTable::GetTypeName(int row,int col){
	return wxGRID_VALUE_STRING; }

sudGridTable::sudGridTable( int nbx, int nby )
: wxGridTableBase( ), sudEditor(nbx,nby)
{
	m_attrInitial = new wxGridCellAttr();
	m_attrInitial->SetTextColour(*wxBLACK);
	m_attrSolved = new wxGridCellAttr();
	m_attrSolved->SetTextColour(*wxGREEN);
	m_attrInvalid = new wxGridCellAttr( );
	m_attrInvalid->SetBackgroundColour( *wxRED );
	m_attrUser = new wxGridCellAttr();
	m_attrUser->SetTextColour(*wxLIGHT_GREY);
	m_attrFalseInput = new wxGridCellAttr( *wxRED, *wxWHITE, wxNullFont, wxALIGN_CENTRE , wxALIGN_CENTRE );
}

sudGridTable::~sudGridTable()
{
	if(!GetAttrProvider())	// avoid program crash! unfortunately current implementation of wxGridTableBase::~wxGridTableBase is virtual and deletes m_attrProvider without checking if it exists
		SetAttrProvider(new wxGridCellAttrProvider);
	m_attrInitial->DecRef();
	m_attrSolved->DecRef();
	m_attrUser->DecRef();
	m_attrFalseInput->DecRef();
	m_attrInvalid->DecRef();
}

wxGridCellAttr * sudGridTable::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind){
	if(CanHaveAttributes())
		return wxGridTableBase::GetAttr(row,col,kind);
	wxGridCellAttr* attr = GetModeAttr( GetCellMode(col,row) );
	if(!attr)
		return 0;
	// todo : improve font service
	if(!attr->HasFont())
		attr->SetFont(GetView()->GetFont());

	attr->SetReadOnly(IsReadOnly(col,row));	/* todo: anderst machen */
	return attr; }


wxGridCellAttr* sudGridTable::GetModeAttr(int mode){
	wxGridCellAttr* attr;
	switch( mode & _cellmodes){
	case sudEditor::cellmode_riddle:
		attr = m_attrInitial;
		break;
	case sudEditor::cellmode_solution:
		attr = m_attrSolved;
		break;
	case sudEditor::cellmode_user_correct:
		attr = m_attrUser;
		break;
	case sudEditor::cellmode_user_incorrect:
		attr = m_attrFalseInput;
		break;
	default:
		attr = 0;
	}
	attr = attr->Clone();
	if(mode & cellmode_invalid)
		attr->MergeWith(m_attrInvalid);
	return attr;
}


void sudGridTable::SetValue(int row, int col, const wxString &s){
	long lng=0;
	if( s.IsEmpty() || s.ToLong(&lng) )
		SetValueAsLong(row,col, lng);
}

wxString sudGridTable::GetValue(int row, int col){
	if(IsEmptyCell(row,col))
		return wxEmptyString;
	return wxString::Format(wxT("%ld"), GetValueAsLong(row,col)); }


void sudGridTable::NotifyUpdateSize(){
	int dr = GetNumberRows() - GetView()->GetNumberRows();
	int dc = GetNumberCols() - GetView()->GetNumberCols();
	if(dr){
		wxGridTableMessage msg( this, dr<0?wxGRIDTABLE_NOTIFY_ROWS_DELETED:wxGRIDTABLE_NOTIFY_ROWS_INSERTED,0, dr<0?-dr:dr );
        GetView()->ProcessTableMessage( msg );
    }
	if(dc){
		wxGridTableMessage msg( this, dc<0?wxGRIDTABLE_NOTIFY_COLS_DELETED:wxGRIDTABLE_NOTIFY_COLS_INSERTED,0, dc<0?-dc:dc );
        GetView()->ProcessTableMessage( msg );
	}
	if(dr||dc)
		((sudGrid*)GetView())->UpdateDimensions(); }

void sudGridTable::NotifyUpdateCell(int row,int col){
//	GetView()->SendEvent( wxEVT_GRID_CELL_CHANGE, row, col ) ;
}




bool sudGridTable::SetSize(int nbx, int nby){
	int oldhigh = GetSize()->high();

	if( ! sudEditor::SetSize(nbx,nby) )
		return false;

	if( GetSize()->high() - oldhigh )
		NotifyUpdateSize();
	GetView()->Refresh();
	return true;
}


bool sudGridTable::EnterInitMode(bool on){
	return sudEditor::EnterInitMode(on); }

bool sudGridTable::SetCell(int x, int y, int val){
	bool ok = GetSolver().ok();
	if( !sudEditor::SetCell(x,y,val) )
		return false;
	if( !GetSolver().ok() || !ok ){
		wxGridTableMessage msg( this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES );
		GetView()->ProcessTableMessage( msg ); }

	NotifyUpdateCell(x,y);
	if( !IsInitMode() && !GetUnsolved() && *GetSolution()==*GetSolution() ){
		wxMessageBox( wxT("Congratulations!\nYou have successfully solved the sudoku!"),
					wxT("wxSudoku"),wxOK|wxCENTRE,GetView());
	}
	return true; }


void sudGridTable::SetShowSolution(bool on){
	bool old = GetShowSolution();
	sudEditor::SetShowSolution(on);
	if( old != on ){
		wxGridTableMessage msg( this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES );
        GetView()->ProcessTableMessage( msg );
	}
}

void sudGridTable::RefreshProcAll(){
	sudEditor::RefreshProcAll();
	if(GetShowSolution()){
		wxGridTableMessage msg( this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES );		// todo: only get the values , that have changed
        GetView()->ProcessTableMessage( msg );
	}

}
