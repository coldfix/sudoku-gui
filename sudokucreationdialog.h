/////////////////////////////////////////////////////////////////////////////
// Name:        sudokucreationdialog.h
// Purpose:     
// Author:      Thomas Gl��le
// Modified by: 
// Created:     27/07/2007 13:12:59
// RCS-ID:      
// Copyright:   Thomas Gl��le (C) 2007
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

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SUDOKUCREATIONDIALOG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SUDOKUCREATIONDIALOG_TITLE _("Sudoku Solver")
#define SYMBOL_SUDOKUCREATIONDIALOG_IDNAME ID_SUDOKUCREATIONDIALOG
#define SYMBOL_SUDOKUCREATIONDIALOG_SIZE wxSize(399, 299)
#define SYMBOL_SUDOKUCREATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SudokuCreationDialog class declaration
 */

class SudokuCreationDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SudokuCreationDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SudokuCreationDialog();
    SudokuCreationDialog( wxWindow* parent, wxWindowID id = SYMBOL_SUDOKUCREATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDOKUCREATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDOKUCREATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SUDOKUCREATIONDIALOG_SIZE, long style = SYMBOL_SUDOKUCREATIONDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SUDOKUCREATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SUDOKUCREATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SUDOKUCREATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SUDOKUCREATIONDIALOG_SIZE, long style = SYMBOL_SUDOKUCREATIONDIALOG_STYLE );

    /// Destructor
    ~SudokuCreationDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	///
	bool CustomDimensionsDialog();

////@begin SudokuCreationDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIO_DIM_CUST
    void OnRadioDimCustSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIM_CUST
    void OnButtonDimCustClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHECK_GEN_ACTIVATE
    void OnCheckGenActivateUpdate( wxUpdateUIEvent& event );

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

    wxSize GetCustomDimensions() const { return m_custom_dimensions ; }
    void SetCustomDimensions(wxSize value) { m_custom_dimensions = value ; }

    wxSize GetDimensions() const { return m_dimensions ; }
    void SetDimensions(wxSize value) { m_dimensions = value ; }

    bool GetGenerateSudoku() const { return m_generate_sudoku ; }
    void SetGenerateSudoku(bool value) { m_generate_sudoku = value ; }

    int GetDifficulty() const { return m_difficulty ; }
    void SetDifficulty(int value) { m_difficulty = value ; }

    int GetDimensionsSelectedIndex() const { return m_dimensions_selectedindex ; }
    void SetDimensionsSelectedIndex(int value) { m_dimensions_selectedindex = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SudokuCreationDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SudokuCreationDialog member variables
    wxRadioButton* m_radio_dim_cust;
    wxCheckBox* m_check_generate;
    wxSize m_custom_dimensions;
    wxSize m_dimensions;
    bool m_generate_sudoku;
    int m_difficulty;
    int m_dimensions_selectedindex;
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



#include<gwx/gwxValidators.h>

class sudRadioValidator : public gwxIndexedRadioButtonValidator<wxSize,int>{

//	DECLARE_DYNAMIC_CLASS(sudRadioValidator)
//	DECLARE_EVENT_TABLE()

public:

	///

    // Constructors
	sudRadioValidator() { }

    // Constructors
	sudRadioValidator(ValueType *selectval,const ValueType&persval,
					IndexType *selectidx,const IndexType&persidx)
					: gwxIndexedRadioButtonValidator(selectval,persval,selectidx,persidx)
	{
	}

	sudRadioValidator(const sudRadioValidator& val)
		: gwxIndexedRadioButtonValidator(val)
	{
	}

    // Destructor
    ~sudRadioValidator() { }

    // Make a clone of this validator
    virtual wxObject *Clone() const { return new sudRadioValidator(*this); }

    // Copies val to this object
	bool Copy(const sudRadioValidator& val){
		return gwxIndexedRadioButtonValidator::Copy(val); }

    // Called to transfer data to the window
    virtual bool TransferToWindow();
};






#endif
    // _SUDOKUCREATIONDIALOG_H_
