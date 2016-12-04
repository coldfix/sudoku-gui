/////////////////////////////////////////////////////////////////////////////
// Name:        sudokucreationdialog.h
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     27/07/2007 13:12:59
// RCS-ID:
// Copyright:   Thomas Gläßle (C) 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 27/07/2007 13:12:59

#ifndef _SUDOKUCREATIONDIALOG_H_
#define _SUDOKUCREATIONDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sudokucreationdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
////@end includes

#include "sudFrame.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

class sudRadioDeliverer;
class sudGenData;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SUDOKUCREATIONDIALOG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SUDOKUCREATIONDIALOG_TITLE _("wxSudoku")
#define SYMBOL_SUDOKUCREATIONDIALOG_IDNAME ID_SUDOKUCREATIONDIALOG
#define SYMBOL_SUDOKUCREATIONDIALOG_SIZE wxSize(399, 299)
#define SYMBOL_SUDOKUCREATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SudokuCreationDialog class declaration
 */

class SudokuCreationDialog: public wxDialog
{
    DECLARE_DYNAMIC_CLASS(SudokuCreationDialog)
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SudokuCreationDialog();
    SudokuCreationDialog(wxWindow* parent, wxWindowID id = SYMBOL_SUDOKUCREATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDOKUCREATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDOKUCREATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SUDOKUCREATIONDIALOG_SIZE, long style = SYMBOL_SUDOKUCREATIONDIALOG_STYLE);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_SUDOKUCREATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDOKUCREATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDOKUCREATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SUDOKUCREATIONDIALOG_SIZE, long style = SYMBOL_SUDOKUCREATIONDIALOG_STYLE);

    /// Destructor
    ~SudokuCreationDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    ///
    bool CustomDimensionsDialog();

    sudGenData& GetData();

////@begin SudokuCreationDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIO_DIM_CUST
    void OnRadioDimCustSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIM_CUST
    void OnButtonDimCustClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_RADIO_DIFF_EASY
    void OnRadioDiffEasyUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_RADIO_DIFF_MEDIUM
    void OnRadioDiffMediumUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_RADIO_DIFF_HARD
    void OnRadioDiffHardUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end SudokuCreationDialog event handler declarations

////@begin SudokuCreationDialog member function declarations

    /// Data manager window access
    sudFrame* GetDataWindow() { wxWindow* w = this; while (w && !w->IsKindOf(CLASSINFO(sudFrame))) { w = w->GetParent(); }; return (sudFrame*) w; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SudokuCreationDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

    sudRadioDeliverer* m_customdeliverer;

////@begin SudokuCreationDialog member variables
    wxRadioButton* m_radio_dim_cust;
    wxCheckBox* m_check_generate;
    /// Control identifiers
    enum {
        ID_SUDOKUCREATIONDIALOG = 10007,
        ID_RADIO_DIM_1 = 10000,
        ID_RADIO_DIM_2 = 10001,
        ID_RADIO_DIM_3 = 10002,
        ID_RADIO_DIM_CUST = 10016,
        ID_BUTTON_DIM_CUST = 10010,
        ID_CHECK_GEN_ACTIVATE = 10012,
        ID_RADIO_DIFF_EASY = 10013,
        ID_RADIO_DIFF_MEDIUM = 10014,
        ID_RADIO_DIFF_HARD = 10015
    };
////@end SudokuCreationDialog member variables
};



#include "wxtk/validators.h"

class sudRadioDeliverer : public wxtk::IndexCheckDeliverer<wxSize>{
public:
    sudRadioDeliverer(ValueType *selectval,const ValueType&persval,
                    IndexType *selectidx,const IndexType&persidx)
        : wxtk::IndexCheckDeliverer(selectval,persval,selectidx,persidx)
    {
    }

    sudRadioDeliverer(const sudRadioDeliverer& val)
        : wxtk::IndexCheckDeliverer(val)
    {
    }

    ~sudRadioDeliverer() { }

    bool DeliverTo(wxWindow*);
};






#endif
    // _SUDOKUCREATIONDIALOG_H_
