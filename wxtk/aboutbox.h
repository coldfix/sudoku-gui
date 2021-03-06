/////////////////////////////////////////////////////////////////////////////
// Name:        aboutbox.h
// Purpose:
// Author:      Thomas Gl��le
// Modified by:
// Created:     Fri 25 Feb 2011 01:54:46 AM CET
// RCS-ID:
// Copyright:   Thomas Gl��le (C) 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Fri 25 Feb 2011 01:54:46 AM CET

#ifndef _ABOUTBOX_H_
#define _ABOUTBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "aboutbox.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_ABOUTBOX_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ABOUTBOX_TITLE _("About")
#define SYMBOL_ABOUTBOX_IDNAME ID_ABOUTBOX
#define SYMBOL_ABOUTBOX_SIZE wxDefaultSize
#define SYMBOL_ABOUTBOX_POSITION wxDefaultPosition
////@end control identifiers

namespace wxtk
{


/*!
 * AboutBox class declaration
 */

class AboutBox: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( AboutBox )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutBox();
    AboutBox( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTBOX_IDNAME, const wxString& caption = SYMBOL_ABOUTBOX_TITLE, const wxPoint& pos = SYMBOL_ABOUTBOX_POSITION, const wxSize& size = SYMBOL_ABOUTBOX_SIZE, long style = SYMBOL_ABOUTBOX_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTBOX_IDNAME, const wxString& caption = SYMBOL_ABOUTBOX_TITLE, const wxPoint& pos = SYMBOL_ABOUTBOX_POSITION, const wxSize& size = SYMBOL_ABOUTBOX_SIZE, long style = SYMBOL_ABOUTBOX_STYLE );

    /// Destructor
    ~AboutBox();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutBox event handler declarations

    /// wxEVT_COMMAND_TEXT_URL event handler for wxID_TEXTCTRL_DESCRIPTION
    void OnTextctrlDescriptionClickUrl( wxTextUrlEvent& event );

////@end AboutBox event handler declarations

////@begin AboutBox member function declarations

    wxString GetCopyright() const { return m_copyright ; }
    void SetCopyright(wxString value) { m_copyright = value ; }

    wxString GetDescription() const { return m_description ; }
    void SetDescription(wxString value) { m_description = value ; }

    wxBitmap GetBigIcon() const { return m_bigicon ; }
    void SetBigIcon(wxBitmap value) { m_bigicon = value ; }

    wxString GetAppname() const { return m_appname ; }
    void SetAppname(wxString value) { m_appname = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutBox member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

    bool TransferDataToWindow();


////@begin AboutBox member variables
    wxStaticBitmap* m_ctrl_bitmap;
    wxTextCtrl* m_ctrl_description;
    wxString m_copyright;
    wxString m_description;
    wxBitmap m_bigicon;
    wxString m_appname;
    /// Control identifiers
    enum {
        ID_ABOUTBOX = 10005,
        wxID_STATICBITMAP_BIGICON = 10010,
        wxID_STATIC_APPNAME = 10008,
        wxID_STATIC_COPYRIGHT = 10009,
        wxID_TEXTCTRL_DESCRIPTION = 10007
    };
////@end AboutBox member variables
};

} // ns wxtk

#endif
    // _ABOUTBOX_H_
