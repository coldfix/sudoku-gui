/////////////////////////////////////////////////////////////////////////////
// Name:        sudApp.h
// Purpose:     
// Author:      Thomas Gl‰ﬂle
// Modified by: 
// Created:     16/07/2007 18:44:00
// RCS-ID:      
// Copyright:   Thomas Gl‰ﬂle (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SUDAPP_H_
#define _SUDAPP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sudApp.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "sudFrame.h"
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
////@end control identifiers

/*!
 * sudApp class declaration
 */

class sudApp: public wxApp
{    
    DECLARE_CLASS(sudApp)
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    sudApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin sudApp event handler declarations

////@end sudApp event handler declarations

////@begin sudApp member function declarations

////@end sudApp member function declarations

////@begin sudApp member variables
////@end sudApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(sudApp)
////@end declare app

#endif
    // _SUDAPP_H_
