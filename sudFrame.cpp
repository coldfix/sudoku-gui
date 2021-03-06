/////////////////////////////////////////////////////////////////////////////
// Name:        sudFrame.cpp
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     16/07/2007 18:43:18
// RCS-ID:
// Copyright:   Thomas Gläßle (C) 2007
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
#include "sudgrid.h"
////@end includes

#include "sudFrame.h"
#include "sudEditor.h"
#include "sudGen.h"
#include "SudokuSave.h"

#include "wxtk/aboutbox.h"
#include "sudokucreationdialog.h"
#include "waitdialog.h"

#include <wx/filename.h>
#include <wx/filedlg.h>
#include <wx/dnd.h>         /*drop target*/



////@begin XPM images
#include "icon1.xpm"
////@end XPM images
#include "symbol.xpm"

#include "about.h"



/*!
 * sudFrame type definition
 */

IMPLEMENT_CLASS(sudFrame, wxFrame)


/*!
 * sudFrame event table definition
 */

BEGIN_EVENT_TABLE(sudFrame, wxFrame)

    EVT_MENU_RANGE(ID_MENUITEM_VIEW_SMALL, ID_MENUITEM_VIEW_BIG, sudFrame::OnMenuitemViewSizeClick)
    EVT_MENU_RANGE(ID_MENUITEM_EASY, ID_MENUITEM_HARD, sudFrame::OnMenuitemDifficultyClick)

////@begin sudFrame event table entries
    EVT_MENU( wxID_NEW, sudFrame::OnNewClick )

    EVT_MENU( wxID_OPEN, sudFrame::OnOpenClick )

    EVT_MENU( wxID_SAVE, sudFrame::OnSaveClick )

    EVT_MENU( wxID_EXIT, sudFrame::OnExitClick )

    EVT_UPDATE_UI( ID_MENUITEM_EASY, sudFrame::OnMenuitemEasyUpdate )

    EVT_UPDATE_UI( ID_MENUITEM_MEDIUM, sudFrame::OnMenuitemMediumUpdate )

    EVT_UPDATE_UI( ID_MENUITEM_HARD, sudFrame::OnMenuitemHardUpdate )

    EVT_MENU( ID_MENUITEM_REFILL, sudFrame::OnMenuitemRefillClick )

    EVT_MENU( ID_MENUITEM_GENERATE, sudFrame::OnMenuitemGenerateClick )

    EVT_MENU( ID_MENUITEM_CLEAR, sudFrame::OnMenuitemClearClick )

    EVT_UPDATE_UI( ID_MENUITEM_VIEW_SMALL, sudFrame::OnMenuitemViewSmallUpdate )

    EVT_UPDATE_UI( ID_MENUITEM_VIEW_NORMAL, sudFrame::OnMenuitemViewNormalUpdate )

    EVT_UPDATE_UI( ID_MENUITEM_VIEW_BIG, sudFrame::OnMenuitemViewBigUpdate )

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

sudFrame::sudFrame(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * sudFrame creator
 */

bool sudFrame::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
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
    sudGen::UnInit();
////@begin sudFrame destruction
////@end sudFrame destruction
}


/*!
 * Member initialisation
 */

void sudFrame::Init()
{
////@begin sudFrame member initialisation
    m_button_showsolution = NULL;
    m_button_entersudoku = NULL;
////@end sudFrame member initialisation

    sudGen::Init();
}


/*!
 * Control creation for sudFrame
 */

void sudFrame::CreateControls()
{

////@begin sudFrame content construction
    // Generated by DialogBlocks, 08/05/2008 18:35:51 (Personal Edition)

    sudFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_NEW, _("&New\tCtrl+N"), _("Create a new sudoku field with specified size"), wxITEM_NORMAL);
    itemMenu3->Append(wxID_OPEN, _("&Open\tCtrl+O"), _("Load a sudoku from the local file system"), wxITEM_NORMAL);
    itemMenu3->Append(wxID_SAVE, _("&Save\tCtrl+S"), _("Save current sudoku to the local file system"), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("&Exit\tEsc"), _("Exit the application"), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu9 = new wxMenu;
    itemMenu9->Append(ID_MENUITEM_EASY, _("&Easy\tF1"), _T(""), wxITEM_CHECK);
    itemMenu9->Append(ID_MENUITEM_MEDIUM, _("&Medium\tF2"), _T(""), wxITEM_CHECK);
    itemMenu9->Append(ID_MENUITEM_HARD, _("&Hard\tF3"), _T(""), wxITEM_CHECK);
    itemMenu9->AppendSeparator();
    itemMenu9->Append(ID_MENUITEM_REFILL, _("&Refill\tF4"), _T(""), wxITEM_NORMAL);
    itemMenu9->Append(ID_MENUITEM_GENERATE, _("&Generate\tF5"), _T(""), wxITEM_NORMAL);
    itemMenu9->AppendSeparator();
    itemMenu9->Append(ID_MENUITEM_CLEAR, _("&Clear\tF6"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu9, _("&Sudoku"));
    wxMenu* itemMenu18 = new wxMenu;
    itemMenu18->Append(ID_MENUITEM_VIEW_SMALL, _("&Small"), _T(""), wxITEM_CHECK);
    itemMenu18->Append(ID_MENUITEM_VIEW_NORMAL, _("&Medium"), _T(""), wxITEM_CHECK);
    itemMenu18->Check(ID_MENUITEM_VIEW_NORMAL, true);
    itemMenu18->Append(ID_MENUITEM_VIEW_BIG, _("&Large"), _T(""), wxITEM_CHECK);
    menuBar->Append(itemMenu18, _("&View"));
    wxMenu* itemMenu22 = new wxMenu;
    itemMenu22->Append(wxID_ABOUT, _("&About...\tF9"), _("About Sudoku-Solver"), wxITEM_NORMAL);
    menuBar->Append(itemMenu22, _("&?"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemFrame1->SetSizer(itemBoxSizer24);

    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer24->Add(itemBoxSizer25, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 25);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer25->Add(itemBoxSizer26, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 10);

    m_sudgrid = new sudGrid( itemFrame1, sudGrid::ID_GRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
    m_sudgrid->SetDefaultColSize(0);
    m_sudgrid->SetDefaultRowSize(0);
    m_sudgrid->SetColLabelSize(0);
    m_sudgrid->SetRowLabelSize(0);
    m_sudgrid->CreateGrid(3, 3, wxGrid::wxGridSelectCells);
    itemBoxSizer26->Add(m_sudgrid, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer28, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_button_showsolution = new wxToggleButton( itemFrame1, ID_SOLVER_SHOW_SOLUTION, _("&Show Solution"), wxDefaultPosition, wxDefaultSize, 0 );
    m_button_showsolution->SetValue(false);
    m_button_showsolution->SetHelpText(_("Blends in the solution of the sudoku"));
    if (sudFrame::ShowToolTips())
        m_button_showsolution->SetToolTip(_("Blends in the solution of the sudoku"));
    itemBoxSizer28->Add(m_button_showsolution, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton30 = new wxButton( itemFrame1, ID_SOLVER_GENERATE, _("&Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton30->SetHelpText(_("Generate a new sudoku"));
    if (sudFrame::ShowToolTips())
        itemButton30->SetToolTip(_("Generate a new sudoku"));
    itemBoxSizer28->Add(itemButton30, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_button_entersudoku = new wxToggleButton( itemFrame1, ID_SOLVER_ENTER_SUDOKU, _("&Edit Sudoku"), wxDefaultPosition, wxDefaultSize, 0 );
    m_button_entersudoku->SetValue(false);
    m_button_entersudoku->SetHelpText(_("Enter your own numbers to create a riddle"));
    if (sudFrame::ShowToolTips())
        m_button_entersudoku->SetToolTip(_("Enter your own numbers to create a riddle"));
    itemBoxSizer28->Add(m_button_entersudoku, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStatusBar* itemStatusBar32 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar32->SetFieldsCount(1);
    itemFrame1->SetStatusBar(itemStatusBar32);

////@end sudFrame content construction

    m_sudgrid->setdisplaysize(1);

    /* drop target */
    class sudDropTarget : public wxFileDropTarget{
    public:
        sudDropTarget(sudFrame* droptarget) { m_droptarget = droptarget; }
        bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames){
            size_t nFiles = filenames.GetCount();
            if (nFiles != 1)        // we're accepting the drop of only one file
                return false;
            m_droptarget->LoadFile(filenames[0]);
            return true; }
    private:
        sudFrame* m_droptarget;
    };
    SetDropTarget(new sudDropTarget(this));
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_NEW
 */

void sudFrame::OnNewClick(wxCommandEvent& event)
{
    SudokuCreationDialog crtdlg(this);
    if (crtdlg.ShowModal() == wxID_OK)
    {
        SetCurrentFile(wxEmptyString);
        wxSize dim = GetData().GetDimensions();
        bool enlarged = dim.x * dim.y > GetEditor()->GetSize().high();
        m_sudgrid->NewGrid(dim.x, dim.y);
        if (enlarged)
            Centre();

        if (GetData().GetGenerateSudoku())
            sudGenerate();
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */

void sudFrame::OnOpenClick(wxCommandEvent& event)
{
    wxFileDialog fdlg(this, wxT("Load file"),
                      wxFileName(GetCurrentFile()).GetPath(),
                      wxFileName(GetCurrentFile()).GetName(),
                      wxT("Sudoku Html file|*.html|Sudoku text file|*.txt"),
                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (fdlg.ShowModal() == wxID_OK)
        LoadFile(fdlg.GetPath());
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE
 */

void sudFrame::OnSaveClick(wxCommandEvent& event)
{
    wxFileDialog fdlg(this, wxT("Save to"),
                      wxFileName(GetCurrentFile()).GetPath(),
                      wxFileName(GetCurrentFile()).GetName(),
                      wxT("Html file|*.html|Text file|*.txt"),
                      wxFD_OVERWRITE_PROMPT | wxFD_SAVE);

    if (fdlg.ShowModal() == wxID_OK)
    {
        SudokuSave* file = fdlg.GetFilterIndex() == 0
                                ? new SudokuSaveHtml
                                : new SudokuSave;

        if (!file->open(fdlg.GetPath()))
        {
            wxMessageBox(wxT("Could not open file for writing!"),
                         wxTheApp->GetAppName(),
                         wxOK | wxCENTRE |wxICON_ERROR,
                         this);
        }
        else
        {
            SetCurrentFile(fdlg.GetPath());

            file -> save (GetEditor() -> GetShown());
        }

        delete file;
    }
}


void sudFrame::LoadFile(const wxString& filename)
{
    SudokuLoad* file = wxFileName(filename).GetExt() == "html"
                            ? new SudokuLoadHtml
                            : new SudokuLoad;

    if (!file->open(filename))
    {
            wxMessageBox(wxT("Could not open file for writing!"),
                         wxTheApp->GetAppName(),
                         wxOK | wxCENTRE |wxICON_ERROR,
                         this);
    }
    else
    {
        m_generated = false;
        SetCurrentFile(filename);
        mSudoku sudoku;
        if (file -> load(sudoku)
            && sudoku.high() >= 1
            && sudoku.high() <= 100)
        {
            bool enlarged = sudoku.high() > GetEditor()->GetSize().high();
            m_sudgrid -> NewGrid(sudoku.bx(), sudoku.by());
            GetEditor() -> SetShown(sudoku);

            if (enlarged)
                Center();
        }
    }

    delete file;
}



/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void sudFrame::OnExitClick(wxCommandEvent& event)
{
    Close(true);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM3
 */

void sudFrame::OnAboutClick(wxCommandEvent& event)
{
    wxtk::AboutBox dlg(this, wxID_ANY, about_title);
    dlg.SetDescription(g_DescriptionTxt);
    dlg.SetAppname(about_appname);
    dlg.SetCopyright(about_copyright);
    dlg.SetBigIcon(about_icon);
    dlg.ShowModal();
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

wxBitmap sudFrame::GetBitmapResource(const wxString& name)
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

wxIcon sudFrame::GetIconResource(const wxString& name)
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

void sudFrame::OnSolverShowSolutionClick(wxCommandEvent& event)
{
    GetEditor()->SetShowSolution(! GetEditor()->GetShowSolution());
    m_sudgrid->SetFocus();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SOLVER_ENTER_SUDOKU
 */

void sudFrame::OnSolverEnterSudokuClick(wxCommandEvent& event)
{
    if (!GetEditor()->SetEditMode(!GetEditor()->IsEditMode()))
        wxMessageBox(wxT("Could switch modes.\nSudoku might currently not be uniquely solvable."),
                     wxTheApp->GetAppName(),
                     wxOK | wxICON_ERROR | wxCENTRE,
                     this);

    m_generated = false;

    m_sudgrid->SetFocus();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_SOLVER_ENTER_SUDOKU
 */

void sudFrame::OnSolverEnterSudokuUpdate(wxUpdateUIEvent& event)
{
    ((wxToggleButton*) FindWindow(ID_SOLVER_ENTER_SUDOKU))
                        -> SetValue(GetEditor() -> IsEditMode());
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SOLVER_GENERATE
 */

void sudFrame::OnSolverGenerateClick(wxCommandEvent& event)
{
    sudGenerate();
    m_sudgrid->SetFocus();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_SOLVER_SHOW_SOLUTION
 */

void sudFrame::OnSolverShowSolutionUpdate(wxUpdateUIEvent& event)
{
    ((wxToggleButton*) FindWindow(ID_SOLVER_SHOW_SOLUTION))
                        -> SetValue(GetEditor() -> GetShowSolution());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_STATUSBAR1
 */

void sudFrame::OnStatusbar1Update(wxUpdateUIEvent& event)
{
    const sudSolve* solver = & GetEditor()->GetSolver();

    wxString text;

    if (GetEditor()->GetRiddle().getsolved() == 0
        && GetEditor()->GetRiddle().size() != 1)
    { }

    else if (solver->error())
        text = wxT("The sudoku is not valid.");

    else if (solver->multiple())
        text = wxT("The sudoku is ambiguous.");

    else if (solver->toohard())
    { }

    else
    {
        wxChar* txts[] = {wxT("Low difficulty"),
                          wxT("Medium difficulty"),
                          wxT("Hard")};

        text = txts[solver->difficulty()];
    }


    if (m_generated)
        text += wxString::Format(wxT(" (%i ms / %i ms)"),
                                 m_gentime,
                                 m_reducetime);

    SetStatusText(text);
}




/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_VIEW_SMALL
 */

void sudFrame::OnMenuitemViewSizeClick(wxCommandEvent& event)
{
    m_sudgrid->setdisplaysize(event.GetId() - ID_MENUITEM_VIEW_SMALL);
    GetSizer()->SetSizeHints(this);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_VIEW_SMALL
 */

void sudFrame::OnMenuitemViewSmallUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_sudgrid->getdisplaysize() == 0);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_VIEW_NORMAL
 */

void sudFrame::OnMenuitemViewNormalUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_sudgrid->getdisplaysize() == 1);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_VIEW_BIG
 */

void sudFrame::OnMenuitemViewBigUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_sudgrid->getdisplaysize() == 2);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_EASY
 */

void sudFrame::OnMenuitemEasyUpdate( wxUpdateUIEvent& event )
{
    event.Check(GetData().GetDifficulty() == sudGen::difficulty_easy);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_MEDIUM
 */

void sudFrame::OnMenuitemMediumUpdate( wxUpdateUIEvent& event )
{
    event.Check(GetData().GetDifficulty() == sudGen::difficulty_medium);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_HARD
 */

void sudFrame::OnMenuitemHardUpdate( wxUpdateUIEvent& event )
{
    event.Check(GetData().GetDifficulty() == sudGen::difficulty_hard);
}



/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for [ID_MENUITEM_EASY, ID_MENUITEM_HARD]
 */

void sudFrame::OnMenuitemDifficultyClick( wxCommandEvent& event )
{
    int difficulty = event.GetId() - ID_MENUITEM_EASY;
    GetData() . SetDifficulty(difficulty);
    sudGenerate(_mutate);
}



/*!
 * Transfer data to the window
 */

bool sudFrame::TransferDataToWindow()
{
    return wxFrame::TransferDataToWindow();
}

/*!
 * Transfer data from the window
 */

bool sudFrame::TransferDataFromWindow()
{
    return wxFrame::TransferDataFromWindow();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_GENERATE
 */

void sudFrame::OnMenuitemGenerateClick( wxCommandEvent& event )
{
    sudGenerate(_regen);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_CLEAR
 */

void sudFrame::OnMenuitemClearClick( wxCommandEvent& event )
{
    GetEditor() -> ClearSudoku();
    m_generated = false;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_REFILL
 */

void sudFrame::OnMenuitemRefillClick( wxCommandEvent& event )
{
    sudGenerate(_refill);
}







sudEditor* sudFrame::GetEditor() const{
    return m_sudgrid->GetEditor(); }





void sudFrame::sudGenerate(int action)
{
    bool success;

    sudGen generator(GetEditor()->GetSize().bx(),
                    GetEditor()->GetSize().by());

    if (action == _mutate || action == _refill)
    {
        if (!GetEditor()->GetSolver().solved()
            || GetEditor()->GetSolver().error())
            return;

        generator.setriddle(GetEditor()->GetRiddle());
    }

    if (GetEditor()->GetSize().high() >= 18)
    {
        sudWaitDialog* waitdialog = new sudWaitDialog(&generator,this);
        waitdialog->Show(true);
        {
            wxWindowDisabler disabler(waitdialog);

            if (action == _mutate)
            {
            }
            else
            {
                success = generator.generate(GetData() . GetDifficulty());
            }
        }
        waitdialog->Close();
    }
    else
    {
        if (action == _mutate)
        {
        }
        else
        {
            success = generator.generate(GetData() . GetDifficulty());
        }
    }

    if (success)
    {
        m_sudgrid->SetSudoku(generator.GetSudoku());
        SetCurrentFile(wxEmptyString);
        m_generated = true;

        if (action == _regen)
        {
            m_gentime = generator.gentime();
            m_reducetime = generator.reducetime();
        }
        else if (action == _mutate)
        {
            m_reducetime += generator.reducetime();
        }

        else if (action == _refill)
        {
            m_reducetime = generator.reducetime();
        }


        char* dfclty[] = {"easy", "medium", "hard"};
        wxString spec = wxString::Format("sudoku %i x %i - %s",
                                         GetData().GetDimensions().x,
                                         GetData().GetDimensions().y,
                                         dfclty[GetEditor() -> GetSolver().difficulty()]);
        SetCurrentFile(spec);
    }
}




