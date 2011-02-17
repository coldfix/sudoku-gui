/////////////////////////////////////////////////////////////////////////////
// Name:        sudApp.cpp
// Purpose:     
// Author:      Thomas Gläßle
// Modified by: 
// Created:     16/07/2007 18:44:00
// RCS-ID:      
// Copyright:   Thomas Gläßle (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sudApp.h"
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

#include "sudApp.h"

////@begin XPM images
////@end XPM images


//	avoid load of unneeded resource-intensive handlers

#undef wxUSE_LIBPNG
#undef wxUSE_LIBJPEG
#undef wxUSE_GIF
#undef wxUSE_XPM				/* why isnt this needed? */
#define wxUSE_LIBPNG	0
#define wxUSE_LIBJPEG	0
#define wxUSE_GIF		0
#define wxUSE_XPM		0


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( sudApp )
////@end implement app


/*!
 * sudApp type definition
 */

IMPLEMENT_CLASS( sudApp, wxApp )


/*!
 * sudApp event table definition
 */

BEGIN_EVENT_TABLE( sudApp, wxApp )

////@begin sudApp event table entries
////@end sudApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for sudApp
 */

sudApp::sudApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void sudApp::Init()
{
////@begin sudApp member initialisation
////@end sudApp member initialisation
}

/*!
 * Initialisation for sudApp
 */

bool sudApp::OnInit()
{    
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif

	SetAppName( wxT("wxSudoku") );

	sudFrame* mainWindow = new sudFrame( NULL, sudFrame::ID_SUDOKUSOLVERFRAME );
	if(argc==2)
		mainWindow->LoadFile(argv[1]);
	else
		mainWindow->sudGenerate();
	mainWindow->Show(true);

    return true;
}


/*!
 * Cleanup for sudApp
 */

int sudApp::OnExit()
{    
////@begin sudApp cleanup
	return wxApp::OnExit();
////@end sudApp cleanup
}

