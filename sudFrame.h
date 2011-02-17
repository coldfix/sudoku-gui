/////////////////////////////////////////////////////////////////////////////
// Name:        sudFrame.h
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     16/07/2007 18:43:18
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 16/07/2007 18:43:18

#ifndef _SUDFRAME_H_
#define _SUDFRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sudFrame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/statusbr.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class sudGrid;
class wxToggleButton;
////@end forward declarations
class sudEditor;
class SudokuCreationDialog;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SUDFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_SUDFRAME_TITLE _("Sudoku Solver")
#define SYMBOL_SUDFRAME_IDNAME ID_SUDOKUSOLVERFRAME
#define SYMBOL_SUDFRAME_SIZE wxDefaultSize
#define SYMBOL_SUDFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * sudFrame class declaration
 */

class sudFrame: public wxFrame
{    
    DECLARE_CLASS( sudFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    sudFrame();
    sudFrame( wxWindow* parent, wxWindowID id = SYMBOL_SUDFRAME_IDNAME, const wxString& caption = SYMBOL_SUDFRAME_TITLE, const wxPoint& pos = SYMBOL_SUDFRAME_POSITION, const wxSize& size = SYMBOL_SUDFRAME_SIZE, long style = SYMBOL_SUDFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SUDFRAME_IDNAME, const wxString& caption = SYMBOL_SUDFRAME_TITLE, const wxPoint& pos = SYMBOL_SUDFRAME_POSITION, const wxSize& size = SYMBOL_SUDFRAME_SIZE, long style = SYMBOL_SUDFRAME_STYLE );

    /// Destructor
    ~sudFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin sudFrame event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_NEW
    void OnNewClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
    void OnOpenClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE
    void OnSaveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnExitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnAboutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SOLVER_SHOW_SOLUTION
    void OnSolverShowSolutionClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_SOLVER_SHOW_SOLUTION
    void OnSolverShowSolutionUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SOLVER_GENERATE
    void OnSolverGenerateClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SOLVER_ENTER_SUDOKU
    void OnSolverEnterSudokuClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_SOLVER_ENTER_SUDOKU
    void OnSolverEnterSudokuUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_STATUSBAR1
    void OnStatusbar1Update( wxUpdateUIEvent& event );

////@end sudFrame event handler declarations

	sudEditor* GetEditor() const;
	void sudGenerate();
	wxString sudSaveHtml();
	void LoadFile(wxString fname);

////@begin sudFrame member function declarations

    wxString GetCurrentDirectory() const { return m_current_directory ; }
    void SetCurrentDirectory(wxString value) { m_current_directory = value ; }

    wxString GetCurrentFile() const { return m_current_file ; }
    void SetCurrentFile(wxString value) { m_current_file = value ; }

    int GetDifficulty() const { return m_difficulty ; }
    void SetDifficulty(int value) { m_difficulty = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end sudFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	SudokuCreationDialog* m_crtdlg;
////@begin sudFrame member variables
    sudGrid* m_sudgrid;
    wxToggleButton* m_button_showsolution;
    wxToggleButton* m_button_entersudoku;
    wxString m_current_directory;
    wxString m_current_file;
    int m_difficulty;
    /// Control identifiers
    enum {
        ID_SUDOKUSOLVERFRAME = 10000,
        ID_SOLVER_SHOW_SOLUTION = 10002,
        ID_SOLVER_GENERATE = 10005,
        ID_SOLVER_ENTER_SUDOKU = 10009,
        ID_STATUSBAR1 = 10006
    };
////@end sudFrame member variables
};

#endif
    // _SUDFRAME_H_