/////////////////////////////////////////////////////////////////////////////
// Name:        sudokucreationdialog.cpp
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     27/07/2007 13:12:59
// RCS-ID:
// Copyright:   Thomas Gläßle (C) 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 27/07/2007 13:12:59

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sudokucreationdialog.h"
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

#include "sudokucreationdialog.h"
#include "wxtk/dualnumberentrydialog.h"
#include "sudGen.h"

////@begin XPM images
////@end XPM images



/*!
 * SudokuCreationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS(SudokuCreationDialog, wxDialog)


/*!
 * SudokuCreationDialog event table definition
 */

BEGIN_EVENT_TABLE(SudokuCreationDialog, wxDialog)

////@begin SudokuCreationDialog event table entries
    EVT_RADIOBUTTON( ID_RADIO_DIM_CUST, SudokuCreationDialog::OnRadioDimCustSelected )

    EVT_BUTTON( ID_BUTTON_DIM_CUST, SudokuCreationDialog::OnButtonDimCustClick )

    EVT_UPDATE_UI( ID_RADIO_DIFF_EASY, SudokuCreationDialog::OnRadioDiffEasyUpdate )

    EVT_UPDATE_UI( ID_RADIO_DIFF_MEDIUM, SudokuCreationDialog::OnRadioDiffMediumUpdate )

    EVT_UPDATE_UI( ID_RADIO_DIFF_HARD, SudokuCreationDialog::OnRadioDiffHardUpdate )

    EVT_BUTTON( wxID_OK, SudokuCreationDialog::OnOkClick )

////@end SudokuCreationDialog event table entries

END_EVENT_TABLE()


/*
    sudRadioValidator: sets the radios label
*/

// Called to transfer data to the window
bool sudRadioDeliverer::DeliverTo(wxWindow* window)
{
    wxSize dim = GetPersonalValue();
    wxString numx = dim.x == -1 ? wxT("?") : wxString::Format(wxT("%i"), dim.x),
            numy = dim.y == -1 ? wxT("?") : wxString::Format(wxT("%i"), dim.y);
    wxString label = wxString::Format(wxT("%s x %s"), numx, numy);
    if (((wxRadioButton*)window)->GetLabel() != label)
        ((wxRadioButton*)window)->SetLabel(label);
    return IndexCheckDeliverer<wxSize>::DeliverTo(window);
}


/*!
 * SudokuCreationDialog constructors
 */

SudokuCreationDialog::SudokuCreationDialog()
{
    Init();
}

SudokuCreationDialog::SudokuCreationDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * SudokuCreationDialog creator
 */

bool SudokuCreationDialog::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
////@begin SudokuCreationDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SudokuCreationDialog creation
    return true;
}


/*!
 * SudokuCreationDialog destructor
 */

SudokuCreationDialog::~SudokuCreationDialog()
{
////@begin SudokuCreationDialog destruction
////@end SudokuCreationDialog destruction
}


/*!
 * Member initialisation
 */

void SudokuCreationDialog::Init()
{
////@begin SudokuCreationDialog member initialisation
    m_radio_dim_cust = NULL;
    m_check_generate = NULL;
////@end SudokuCreationDialog member initialisation
}


/*!
 * Control creation for SudokuCreationDialog
 */

void SudokuCreationDialog::CreateControls()
{
    m_customdeliverer =
        new sudRadioDeliverer(&GetData().m_dimensions,
                              GetData().m_custom_dimensions,
                              &GetData().m_dimensions_selectedindex,
                              3);
////@begin SudokuCreationDialog content construction
    // Generated by DialogBlocks, 04/05/2008 20:18:21 (Personal Edition)

    SudokuCreationDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Dimensions"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);

    wxGridSizer* itemGridSizer5 = new wxGridSizer(1, 3, 0, 0);
    itemStaticBoxSizer4->Add(itemGridSizer5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton6 = new wxRadioButton( itemDialog1, ID_RADIO_DIM_1, _("3 x 3"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    itemRadioButton6->SetValue(true);
    itemGridSizer5->Add(itemRadioButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton7 = new wxRadioButton( itemDialog1, ID_RADIO_DIM_2, _("3 x 2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton7->SetValue(false);
    itemGridSizer5->Add(itemRadioButton7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton8 = new wxRadioButton( itemDialog1, ID_RADIO_DIM_3, _("2 x 3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton8->SetValue(false);
    itemGridSizer5->Add(itemRadioButton8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemStaticBoxSizer4->Add(4, 4, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_radio_dim_cust = new wxRadioButton( itemDialog1, ID_RADIO_DIM_CUST, _("? x ?"), wxDefaultPosition, wxDefaultSize, 0 );
    m_radio_dim_cust->SetValue(false);
    itemStaticBoxSizer4->Add(m_radio_dim_cust, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_BUTTON_DIM_CUST, _("&Custom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer4->Add(itemButton11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Sudoku Generation"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer12, 0, wxGROW|wxALL, 5);

    m_check_generate = new wxCheckBox( itemDialog1, ID_CHECK_GEN_ACTIVATE, _("Generate Sudoku"), wxDefaultPosition, wxDefaultSize, 0 );
    m_check_generate->SetValue(false);
    itemStaticBoxSizer12->Add(m_check_generate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemStaticBoxSizer12->Add(4, 4, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer15 = new wxGridSizer(1, 3, 0, 0);
    itemStaticBoxSizer12->Add(itemGridSizer15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton16 = new wxRadioButton( itemDialog1, ID_RADIO_DIFF_EASY, _("Easy"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    itemRadioButton16->SetValue(false);
    itemGridSizer15->Add(itemRadioButton16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton17 = new wxRadioButton( itemDialog1, ID_RADIO_DIFF_MEDIUM, _("Medium"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton17->SetValue(true);
    itemGridSizer15->Add(itemRadioButton17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxRadioButton* itemRadioButton18 = new wxRadioButton( itemDialog1, ID_RADIO_DIFF_HARD, _("Hard"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton18->SetValue(false);
    itemGridSizer15->Add(itemRadioButton18, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer19 = new wxStdDialogButtonSizer;

    itemBoxSizer3->Add(itemStdDialogButtonSizer19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton20 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton20->SetDefault();
    itemStdDialogButtonSizer19->AddButton(itemButton20);

    wxButton* itemButton21 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer19->AddButton(itemButton21);

    itemStdDialogButtonSizer19->Realize();

    // Set validators
    itemRadioButton6->SetValidator( wxtk::Validator(new sudRadioDeliverer(& GetDataWindow()->GetData().m_dimensions, wxSize(3,3), &GetData().m_dimensions_selectedindex, 0)) );
    itemRadioButton7->SetValidator( wxtk::Validator(new sudRadioDeliverer(& GetDataWindow()->GetData().m_dimensions, wxSize(3,2), &GetData().m_dimensions_selectedindex, 1)) );
    itemRadioButton8->SetValidator( wxtk::Validator(new sudRadioDeliverer(& GetDataWindow()->GetData().m_dimensions, wxSize(2,3), &GetData().m_dimensions_selectedindex, 2)) );
    m_radio_dim_cust->SetValidator( wxtk::Validator(m_customdeliverer) );
    m_check_generate->SetValidator( wxGenericValidator(& GetDataWindow()->GetData().m_generate_sudoku) );
    itemRadioButton16->SetValidator( wxtk::Validator(new wxtk::ValueCheckDeliverer<int>( & GetDataWindow()->GetData().m_difficulty, sudGen::difficulty_easy )) );
    itemRadioButton17->SetValidator( wxtk::Validator(new wxtk::ValueCheckDeliverer<int>( & GetDataWindow()->GetData().m_difficulty, sudGen::difficulty_medium ) ) );
    itemRadioButton18->SetValidator( wxtk::Validator(new wxtk::ValueCheckDeliverer<int>( & GetDataWindow()->GetData().m_difficulty, sudGen::difficulty_hard ) ) );
////@end SudokuCreationDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool SudokuCreationDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SudokuCreationDialog::GetBitmapResource(const wxString& name)
{
    // Bitmap retrieval
////@begin SudokuCreationDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SudokuCreationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SudokuCreationDialog::GetIconResource(const wxString& name)
{
    // Icon retrieval
////@begin SudokuCreationDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SudokuCreationDialog icon retrieval
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON3
 */

void SudokuCreationDialog::OnRadioDiffEasyUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_check_generate->IsChecked());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON5
 */

void SudokuCreationDialog::OnRadioDiffMediumUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_check_generate->IsChecked());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON6
 */

void SudokuCreationDialog::OnRadioDiffHardUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_check_generate->IsChecked());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
 */

void SudokuCreationDialog::OnButtonDimCustClick(wxCommandEvent& event)
{
    CustomDimensionsDialog();
}

sudGenData& SudokuCreationDialog::GetData()
{
    return GetDataWindow()->GetData();
}


/*

*/


bool SudokuCreationDialog::CustomDimensionsDialog()
{
    wxSize dim = GetData().GetCustomDimensions();
    if (dim.x == -1)
        dim.x = 3;
    if (dim.y == -1)
        dim.y = 3;

    wxtk::DualNumberEntryDialog dlg(this,0);
    dlg.SetMessage(wxT("Please specify the dimensions for the sudoku.\nNumber of blocks:"));
    dlg.SetPrompt0(wxT("Columns:"));
    dlg.SetNumber0(wxtk::SpinInfo(dim.x, 1, 10));
    dlg.SetPrompt1(wxT("Rows:"));
    dlg.SetNumber1(wxtk::SpinInfo(dim.y, 1, 10));

    if (dlg.ShowModal() != wxID_OK)
        return false;

    dim = wxSize(dlg.GetNumber0().value,
                 dlg.GetNumber1().value);


    m_customdeliverer->SetPersonalValue(dim);
    GetData().SetCustomDimensions(dim);
    wxString label = wxString::Format(wxT("%i x %i"), dim.x, dim.y);
    m_radio_dim_cust->SetLabel(label);
    m_radio_dim_cust->SetValue(true);
    return true;
}





/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIO_DIM_CUST
 */

void SudokuCreationDialog::OnRadioDimCustSelected(wxCommandEvent& event)
{
    wxSize dim = GetData().GetCustomDimensions();
    if (dim.x == -1 || dim.y == -1)
    {
        if (!CustomDimensionsDialog())
            ((wxRadioButton*) FindWindow(ID_RADIO_DIM_1)) -> SetValue(true);
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SudokuCreationDialog::OnOkClick(wxCommandEvent& event)
{
    bool skip = true;
    if (m_radio_dim_cust->GetValue() && m_check_generate->GetValue())
    {
        if (GetData().GetCustomDimensions().x * GetData().GetCustomDimensions().y > 16)
        {
            if (wxNO == wxMessageBox(
                    wxT("It is NOT recommended to generate sudokus with more than 16 fields per block.\nThis might cost a significant amount of CPU time.\nThe application might be hanging some minutes.\n\nAre you sure?"),
                    wxTheApp->GetAppName(),
                    wxYES|wxNO|wxCENTRE|wxICON_QUESTION))
            {
                skip = false;
            }
        }
    }

    if (skip)
        event.Skip();
}

