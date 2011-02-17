/////////////////////////////////////////////////////////////////////////////
// Name:        sudFrame.cpp
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     16/07/2007 18:43:18
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 16/07/2007 18:43:18

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sudFrame.h"
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
#include "sudGrid.h"
////@end includes

#include "sudFrame.h"
#include "sudEditor.h"

#include <gwx/AboutBox.h>
#include "sudokucreationdialog.h"
#include "sudGen.h"

//#include <wx/stream.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>
#include <wx/filedlg.h>

#include <wx/dnd.h>		/*drop target*/


#include "debug.h"


////@begin XPM images
#include "icon1.xpm"
////@end XPM images
#include "symbol.xpm"


/*!
 * sudFrame type definition
 */

IMPLEMENT_CLASS( sudFrame, wxFrame )


/*!
 * sudFrame event table definition
 */

BEGIN_EVENT_TABLE( sudFrame, wxFrame )

////@begin sudFrame event table entries
    EVT_MENU( wxID_NEW, sudFrame::OnNewClick )

    EVT_MENU( wxID_OPEN, sudFrame::OnOpenClick )

    EVT_MENU( wxID_SAVE, sudFrame::OnSaveClick )

    EVT_MENU( wxID_EXIT, sudFrame::OnExitClick )

    EVT_MENU( wxID_ABOUT, sudFrame::OnAboutClick )

    EVT_TOGGLEBUTTON( ID_SOLVER_SHOW_SOLUTION, sudFrame::OnSolverShowSolutionClick )
    EVT_UPDATE_UI( ID_SOLVER_SHOW_SOLUTION, sudFrame::OnSolverShowSolutionUpdate )

    EVT_BUTTON( ID_SOLVER_GENERATE, sudFrame::OnSolverGenerateClick )

    EVT_TOGGLEBUTTON( ID_SOLVER_ENTER_SUDOKU, sudFrame::OnSolverEnterSudokuClick )
    EVT_UPDATE_UI( ID_SOLVER_ENTER_SUDOKU, sudFrame::OnSolverEnterSudokuUpdate )

    EVT_UPDATE_UI( ID_STATUSBAR1, sudFrame::OnStatusbar1Update )

////@end sudFrame event table entries

END_EVENT_TABLE()


/*!
 * sudFrame constructors
 */

sudFrame::sudFrame()
{
    Init();
}

sudFrame::sudFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * sudFrame creator
 */

bool sudFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin sudFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("icon1.xpm")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end sudFrame creation
    return true;
}


/*!
 * sudFrame destructor
 */

sudFrame::~sudFrame()
{
	delete m_crtdlg;
////@begin sudFrame destruction
////@end sudFrame destruction
}


/*!
 * Member initialisation
 */

void sudFrame::Init()
{
////@begin sudFrame member initialisation
    m_difficulty = sudGen::difficulty_medium;
    m_button_showsolution = NULL;
    m_button_entersudoku = NULL;
////@end sudFrame member initialisation
}


/*!
 * Control creation for sudFrame
 */

void sudFrame::CreateControls()
{
	DEBUG_CREATE_COUNT_VIEW(sudSolveImpl);
	DEBUG_CREATE_COUNT_VIEW(sudSudoku);

////@begin sudFrame content construction
    // Generated by DialogBlocks, 23/08/2007 20:19:37 (Personal Edition)

    sudFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_NEW, _("&New\tCtrl+N"), _("Create a new sudoku field with specified size"), wxITEM_NORMAL);
    itemMenu3->Append(wxID_OPEN, _("&Open\tCtrl+O"), _("Load a sudoku from a text file"), wxITEM_NORMAL);
    itemMenu3->Append(wxID_SAVE, _("&Save\tCtrl+S"), _("Save current sudoku to a text file"), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("&Exit\tEsc"), _("Exit the application"), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&Sudoku"));
    wxMenu* itemMenu9 = new wxMenu;
    itemMenu9->Append(wxID_ABOUT, _("&About...\tF9"), _("About Sudoku-Solver"), wxITEM_NORMAL);
    menuBar->Append(itemMenu9, _("&?"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemFrame1->SetSizer(itemBoxSizer11);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer11->Add(itemBoxSizer12, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 25);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer12->Add(itemBoxSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 10);

    m_sudgrid = new sudGrid( itemFrame1, sudGrid::ID_GRID, wxDefaultPosition, wxDefaultSize, 0 );
    m_sudgrid->SetDefaultColSize(35);
    m_sudgrid->SetDefaultRowSize(35);
    m_sudgrid->SetColLabelSize(0);
    m_sudgrid->SetRowLabelSize(0);
    m_sudgrid->CreateGrid(3, 3, wxGrid::wxGridSelectCells);
    itemBoxSizer13->Add(m_sudgrid, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer13->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_button_showsolution = new wxToggleButton( itemFrame1, ID_SOLVER_SHOW_SOLUTION, _("&Show Solution"), wxDefaultPosition, wxDefaultSize, 0 );
    m_button_showsolution->SetValue(false);
    m_button_showsolution->SetHelpText(_("Blends in the solution of the sudoku"));
    if (sudFrame::ShowToolTips())
        m_button_showsolution->SetToolTip(_("Blends in the solution of the sudoku"));
    itemBoxSizer15->Add(m_button_showsolution, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton17 = new wxButton( itemFrame1, ID_SOLVER_GENERATE, _("&Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton17->SetHelpText(_("Generates a new sudoku using the current dimensions"));
    if (sudFrame::ShowToolTips())
        itemButton17->SetToolTip(_("Generates a new sudoku using the current dimensions"));
    itemBoxSizer15->Add(itemButton17, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_button_entersudoku = new wxToggleButton( itemFrame1, ID_SOLVER_ENTER_SUDOKU, _("&Enter Sudoku"), wxDefaultPosition, wxDefaultSize, 0 );
    m_button_entersudoku->SetValue(false);
    m_button_entersudoku->SetHelpText(_("Create a sudoku by typing it in manually"));
    if (sudFrame::ShowToolTips())
        m_button_entersudoku->SetToolTip(_("Create a sudoku by typing it in manually"));
    itemBoxSizer15->Add(m_button_entersudoku, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStatusBar* itemStatusBar19 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar19->SetFieldsCount(1);
    itemFrame1->SetStatusBar(itemStatusBar19);

////@end sudFrame content construction

	m_sudgrid->SetFont( wxFont(14,wxDEFAULT, wxNORMAL,wxBOLD) );

	/* drop target */
	class sudDropTarget : public wxFileDropTarget{
	public:
		sudDropTarget(sudFrame* droptarget)	{ m_droptarget = droptarget; }
		bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames){
			size_t nFiles = filenames.GetCount();
			if(nFiles!=1)		// we're accepting the drop of only one file
				return false;
			m_droptarget->LoadFile( filenames[0] );
			return true; }
	private:
		sudFrame* m_droptarget;
	};
	SetDropTarget( new sudDropTarget(this) );

	m_crtdlg = new SudokuCreationDialog(this);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_NEW
 */

void sudFrame::OnNewClick( wxCommandEvent& event )
{
	const sudDynSize* psize = GetEditor()->GetSize();
	m_crtdlg->SetDifficulty( GetDifficulty() );
	if(m_crtdlg->ShowModal() == wxID_OK){
		wxSize dim = m_crtdlg->GetDimensions();
		m_sudgrid->GetEditor()->SetSize( dim.x,dim.y );
		SetDifficulty( m_crtdlg->GetDifficulty() );
		if( m_crtdlg->GetGenerateSudoku() ){
			sudGenerate(); }
		SetCurrentFile(wxEmptyString);
	}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */

void sudFrame::OnOpenClick( wxCommandEvent& event )
{
	wxFileDialog fdlg(this,wxT("Load file"),
					GetCurrentDirectory(),GetCurrentFile(),
					wxT("Sudoku field|*.txt"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(fdlg.ShowModal()==wxID_OK){
		LoadFile(fdlg.GetPath());
	}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE
 */

void sudFrame::OnSaveClick( wxCommandEvent& event )
{
	wxFileDialog fdlg(this,wxT("Save to"),
					GetCurrentDirectory(),wxFileName(GetCurrentFile()).GetName(),
					wxT("Text file|*.txt|Html file|*.html"),wxFD_OVERWRITE_PROMPT|wxFD_SAVE);
	if(fdlg.ShowModal()==wxID_OK){
		wxFileName fname = fdlg.GetPath();
		SetCurrentDirectory(fname.GetPath());
		SetCurrentFile(fname.GetFullName());

		wxFileOutputStream fo(fdlg.GetPath());
		if(!fo.Ok()){
			wxMessageBox(wxT("Could not open file for writing!"),wxT("Sudoku Solver"),wxOK|wxCENTRE|wxICON_ERROR,this);
		}else{
			wxTextOutputStream text(fo);
			if(fdlg.GetFilterIndex()==0){
				GetEditor()->GetRiddle()->save(text);	// todo: save not only riddle but merged with user ipt?
			}else{
				text << sudSaveHtml();
			}
		}
	}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void sudFrame::OnExitClick( wxCommandEvent& event )
{
    Close(true);
}


#define USE_GWX_ABOUT

#include <wx/aboutdlg.h>
#include "about.h"

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM3
 */

void sudFrame::OnAboutClick( wxCommandEvent& event )
{
#ifndef USE_GWX_ABOUT
	wxAboutDialogInfo info;
	info.SetDescription(about2_description);
	info.SetName(about2_name);
	info.SetLicence(about2_license);
	info.AddDeveloper(about2_developer);
	info.SetCopyright(about2_copyright);
	info.SetVersion(about2_version);
	info.SetIcon( about2_icon );
	::wxAboutBox(info);
#else
	gwxAboutBox dlg(this,wxID_ANY,about_title );
	dlg.SetDescription(g_DescriptionTxt);
	dlg.SetAppname(about_appname);
	dlg.SetCopyright(about_copyright);
	dlg.SetBigIcon( about_icon );
	dlg.ShowModal();
#endif
}

/*!
 * Should we show tooltips?
 */

bool sudFrame::ShowToolTips()
{
    return true;
}


/*!
 * Get bitmap resources
 */

wxBitmap sudFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    if (name == _T("symbol.xpm"))
    {
        wxBitmap bitmap(symbol_xpm);
        return bitmap;
    }
////@begin sudFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end sudFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon sudFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin sudFrame icon retrieval
    wxUnusedVar(name);
    if (name == _T("icon1.xpm"))
    {
        wxIcon icon(icon1_xpm);
        return icon;
    }
    return wxNullIcon;
////@end sudFrame icon retrieval
}





/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SOLVER_SHOW_SOLUTION
 */

void sudFrame::OnSolverShowSolutionClick( wxCommandEvent& event )
{
	GetEditor()->SetShowSolution( ! GetEditor()->GetShowSolution() );
	m_sudgrid->SetFocus();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SOLVER_ENTER_SUDOKU
 */

void sudFrame::OnSolverEnterSudokuClick( wxCommandEvent& event )
{
	bool ok = GetEditor()->EnterInitMode( ! GetEditor()->IsInitMode() );
	if(GetEditor()->IsInitMode() && !ok)
		wxMessageBox(wxT("Could not switch to User-Mode, since Sudoku is not uniquely solvable!"),
					wxT("Sudoku Solver"), wxOK|wxICON_ERROR|wxCENTRE, this );

	m_sudgrid->SetFocus();
	// todo: error output?
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_SOLVER_ENTER_SUDOKU
 */

void sudFrame::OnSolverEnterSudokuUpdate( wxUpdateUIEvent& event )
{
	//event.Check( GetEditor()->IsInitMode() );		// ! ... ?

	((wxToggleButton*)FindWindow(ID_SOLVER_ENTER_SUDOKU))->SetValue( GetEditor()->IsInitMode() );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SOLVER_GENERATE
 */

void sudFrame::OnSolverGenerateClick( wxCommandEvent& event )
{
	sudGenerate();
	m_sudgrid->SetFocus();
}


void sudFrame::sudGenerate(){
	const sudDynSize* psize = GetEditor()->GetSize();
	sudSudoku* psud = sudGen::generate( psize->b_x(), psize->b_y(), GetDifficulty() );
	if(!psud){
		wxMessageBox( wxT("Could not generate sudoku!"), wxT("Sudoku Solver"), wxOK|wxICON_ERROR|wxCENTRE, this);
		return; }

	SetCurrentFile(wxEmptyString);
	m_sudgrid->InitFromSudoku( *psud );
	delete psud;
}


sudEditor* sudFrame::GetEditor() const{
	return m_sudgrid->GetEditor(); }


/*!
 * wxEVT_UPDATE_UI event handler for ID_SOLVER_SHOW_SOLUTION
 */

void sudFrame::OnSolverShowSolutionUpdate( wxUpdateUIEvent& event )
{
//	event.Check( GetEditor()->GetShowSolution() );		// ! ... ?
	
	((wxToggleButton*)FindWindow(ID_SOLVER_SHOW_SOLUTION))->SetValue( GetEditor()->GetShowSolution() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_STATUSBAR1
 */

void sudFrame::OnStatusbar1Update( wxUpdateUIEvent& event )
{
	sudSolve* solver = GetEditor()->GetSolver();
	int difficulty;

	wxString text;

	if(!GetEditor()->GetRiddle()->getsolved())
		text = wxT("Welcome to Sudoku Solver!");
	else if( ! solver->isok() )
		text = wxT("The sudoku is not valid.");
	else if( ! solver->isunique() )
		text = wxT("The sudoku has more than one solution currently.");
	else if( solver->toohard() )
		text = wxT("The sudoku is too hard to solve at the moment. It probably has lots of solutions.");
	else{	// analyze difficulty:
		int proc = solver->getchild()->getprocneeded();
		if(proc & sudSolveBase::proc_assump )
			text = wxT("This should be really hard!");
		else if(proc & sudSolveBase::proc_clever)
			text = wxT("This should be a hard one!");
		else if(proc & sudSolveBase::proc_smart)
			text = wxT("Medium difficulty.");
		else
			text = wxT("Low difficulty!");
	}

	SetStatusText( text );
}


#include"html_templates.h"


bool hasborder(int num,int dim,int high){
	return !(num%dim) && num && num!=high; }


wxString sudFrame::sudSaveHtml(){
	const sudEditor* ed = GetEditor();
	int fx=ed->GetSize()->f_x(),fy=ed->GetSize()->f_y(),h=fx*fy;
	wxString file = g_TplFileTxt;
	wxString rows;

	for(int y=0;y<h;y++){
		wxString row = g_TplRowTxt;
		wxString cells;
		wxString tbclass = wxString::Format(wxT("%s %s"),hasborder(y,fy,h)?wxT("topborder"):wxEmptyString,
														 hasborder(y+1,fy,h)?wxT("bottomborder"):wxEmptyString);
		tbclass.Trim(0);
		tbclass.Trim(1);
		for(int x=0;x<h;x++){
			wxString cell = g_TplCellTxt;
			wxString number = ed->GetCell(x,y) ? wxString::Format(wxT("%ld"),ed->GetCell(x,y)) : wxEmptyString;
			cell.Replace(wxT("%NUMBER%"), number );

			wxString cellclass, mode;
			switch(ed->GetCellMode(x,y)){
				case sudEditor::cellmode_riddle:
					mode = wxT("riddle");
					break;
				case sudEditor::cellmode_user_correct:
					mode = wxT("user");
					break;
				case sudEditor::cellmode_user_incorrect:
					mode = wxT("incorrect");
					break;
				case sudEditor::cellmode_solution:
					mode = wxT("solution");
					break;
				case sudEditor::cellmode_invalid:
					mode = wxT("invalid");
					break;
			};

			wxString tbrlclass = wxString::Format(wxT("%s %s %s %s"),mode, tbclass,
														hasborder(x,fx,h)?wxT("leftborder"):wxEmptyString,
														hasborder(x+1,fx,h)?wxT("rightborder"):wxEmptyString);
			tbrlclass.Trim(0);
			tbrlclass.Trim(1);
			tbrlclass.Replace(wxT("   "),wxT(" "));
			tbrlclass.Replace(wxT("  "),wxT(" "));
			cell.Replace(wxT("%CLASS%"), tbrlclass );

			cells += cell;
		}

		row.Replace( wxT("%CELLS%"), cells );
		row.Replace( wxT("%CLASS%"), wxEmptyString );	/* we could also pass tbclass here */
		rows += row;
	}

	file.Replace( wxT("%ROWS%"), rows );
	return file; }


void sudFrame::LoadFile(wxString filename){
	wxFileName fname = filename;
	SetCurrentDirectory(fname.GetPath());
	SetCurrentFile(fname.GetFullName());

	sudSudoku sud;
	wxFileInputStream fi( filename );
	wxTextInputStream text(fi);
	sud.load<wxTextInputStream>(text);
	m_sudgrid->InitFromSudoku(sud);

	GetEditor()->EnterInitMode(0);
}







