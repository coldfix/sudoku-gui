/////////////////////////////////////////////////////////////////////////////
// Name:        waitdialog.cpp
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     19/12/2007 19:01:55
// RCS-ID:
// Copyright:   Thomas Gläßle (C) 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 19/12/2007 19:01:55

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "waitdialog.h"
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

#include "waitdialog.h"

#include "sudgen.h"

////@begin XPM images
////@end XPM images


/*!
 * sudWaitDialog type definition
 */

IMPLEMENT_CLASS(sudWaitDialog, wxDialog)


/*!
 * sudWaitDialog event table definition
 */

BEGIN_EVENT_TABLE(sudWaitDialog, wxDialog)

////@begin sudWaitDialog event table entries
    EVT_CLOSE( sudWaitDialog::OnCloseWindow )

    EVT_BUTTON( ID_BUTTON_RESTART, sudWaitDialog::OnButtonRestartClick )

    EVT_BUTTON( wxID_STOP, sudWaitDialog::OnStopClick )

    EVT_BUTTON( wxID_CANCEL, sudWaitDialog::OnCancelClick )

////@end sudWaitDialog event table entries

END_EVENT_TABLE()


/*!
 * sudWaitDialog constructors
 */

sudWaitDialog::sudWaitDialog(sudGen* generator)
{
    m_generator = generator;
    Init();
}

sudWaitDialog::sudWaitDialog(sudGen* generator, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    m_generator = generator;
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * sudWaitDialog creator
 */

bool sudWaitDialog::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
////@begin sudWaitDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end sudWaitDialog creation
    return true;
}


/*!
 * sudWaitDialog destructor
 */

sudWaitDialog::~sudWaitDialog()
{
////@begin sudWaitDialog destruction
////@end sudWaitDialog destruction
}


/*!
 * Member initialisation
 */

void sudWaitDialog::Init()
{
////@begin sudWaitDialog member initialisation
////@end sudWaitDialog member initialisation
}


/*!
 * Control creation for sudWaitDialog
 */

void sudWaitDialog::CreateControls()
{
////@begin sudWaitDialog content construction
    // Generated by DialogBlocks, 07/05/2008 23:55:53 (Personal Edition)

    sudWaitDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Please wait, while wxSudoku generates the sudoku.\nUnfortunately, for large sudokus this can require a significant amount of time.\n(Depending on the size - up to several minutes!)\n\nThank you for your patience"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON_RESTART, _("&Restart"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_STOP, _("&Stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton8->SetDefault();
    itemBoxSizer5->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end sudWaitDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool sudWaitDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap sudWaitDialog::GetBitmapResource(const wxString& name)
{
    // Bitmap retrieval
////@begin sudWaitDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end sudWaitDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon sudWaitDialog::GetIconResource(const wxString& name)
{
    // Icon retrieval
////@begin sudWaitDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end sudWaitDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESTART
 */

void sudWaitDialog::OnButtonRestartClick(wxCommandEvent& event)
{
    if (m_generator)
        m_generator->restart();
}




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void sudWaitDialog::OnCancelClick(wxCommandEvent& event)
{
    if (m_generator)
        m_generator->cancel();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_STOP
 */

void sudWaitDialog::OnStopClick(wxCommandEvent& event)
{
    if (m_generator)
        m_generator->stop();
}




/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_WXSUDOKU
 */

void sudWaitDialog::OnCloseWindow(wxCloseEvent& event)
{
    m_generator = 0;
    Destroy();
}
