/////////////////////////////////////////////////////////////////////////////
// Name:        sudgendata.h
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     03/05/2008 21:57:22
// RCS-ID:
// Copyright:   Thomas Gläßle (C) 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 03/05/2008 21:57:22

#ifndef _SUDGENDATA_H_
#define _SUDGENDATA_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sudgendata.h"
#endif

/*!
 * sudGenData class declaration
 */

class sudGenData: public wxObject
{
    friend class sudFrame;
    DECLARE_DYNAMIC_CLASS( sudGenData )
public:
    /// Default constructor for sudGenData
    sudGenData();

    /// Copy constructor for sudGenData
    sudGenData(const sudGenData& data);

    /// Destructor for sudGenData
    ~sudGenData();

    /// Assignment operator for sudGenData
    void operator=(const sudGenData& data);

    /// Equality operator for sudGenData
    bool operator==(const sudGenData& data) const;

    /// Copy function for sudGenData
    void Copy(const sudGenData& data);

    /// Initialises member variables
    void Init();

////@begin sudGenData member function declarations
    int GetDifficulty() const { return m_difficulty ; }
    void SetDifficulty(int value) { m_difficulty = value ; }

    wxSize GetCustomDimensions() const { return m_custom_dimensions ; }
    void SetCustomDimensions(wxSize value) { m_custom_dimensions = value ; }

    wxSize GetDimensions() const { return m_dimensions ; }
    void SetDimensions(wxSize value) { m_dimensions = value ; }

    bool GetGenerateSudoku() const { return m_generate_sudoku ; }
    void SetGenerateSudoku(bool value) { m_generate_sudoku = value ; }

    int GetDimensionsSelectedindex() const { return m_dimensions_selectedindex ; }
    void SetDimensionsSelectedindex(int value) { m_dimensions_selectedindex = value ; }

////@end sudGenData member function declarations

////@begin sudGenData member variables
    int m_difficulty;
    wxSize m_custom_dimensions;
    wxSize m_dimensions;
    bool m_generate_sudoku;
    int m_dimensions_selectedindex;
////@end sudGenData member variables
};

#endif
    // _SUDGENDATA_H_
