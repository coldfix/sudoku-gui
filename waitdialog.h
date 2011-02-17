/////////////////////////////////////////////////////////////////////////////
// Name:        waitdialog.h
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     19/12/2007 19:01:55
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 19/12/2007 19:01:55

#ifndef _WAITDIALOG_H_
#define _WAITDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "waitdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

class sudGen;


/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SUDWAITDIALOG_STYLE wxTAB_TRAVERSAL
#define SYMBOL_SUDWAITDIALOG_TITLE _("wxSudoku")
#define SYMBOL_SUDWAITDIALOG_IDNAME ID_WXSUDOKU
#define SYMBOL_SUDWAITDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_SUDWAITDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * sudWaitDialog class declaration
 */

class sudWaitDialog: public wxDialog
{    
    DECLARE_CLASS( sudWaitDialog )
    DECLARE_EVENT_TABLE()

	sudGen* m_generator;
public:
    /// Constructors
    sudWaitDialog( sudGen* );
    sudWaitDialog( sudGen*, wxWindow* parent, wxWindowID id = SYMBOL_SUDWAITDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDWAITDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDWAITDIALOG_POSITION, const wxSize& size = SYMBOL_SUDWAITDIALOG_SIZE, long style = SYMBOL_SUDWAITDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SUDWAITDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDWAITDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDWAITDIALOG_POSITION, const wxSize& size = SYMBOL_SUDWAITDIALOG_SIZE, long style = SYMBOL_SUDWAITDIALOG_STYLE );

    /// Destructor
    ~sudWaitDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin sudWaitDialog event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_WXSUDOKU
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESTART
    void OnButtonRestartClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

////@end sudWaitDialog event handler declarations

////@begin sudWaitDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end sudWaitDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin sudWaitDialog member variables
    /// Control identifiers
    enum {
        ID_WXSUDOKU = 10004,
        ID_BUTTON_RESTART = 10008
    };
////@end sudWaitDialog member variables
};

#endif
    // _WAITDIALOG_H_