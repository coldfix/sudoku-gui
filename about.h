
#define lf "\r\n"


#ifdef USE_GWX_ABOUT

#include "description_txt.h"


const wxChar* about_title = wxT("About Sudoku Solver");
const wxChar* about_appname = wxT("Sudoku Solver\nVersion 2.1");
const wxChar* about_copyright = wxT("(C) 2007 Thomas Gl‰ﬂle");

#define about_icon GetBitmapResource(wxT("symbol.xpm"))


#else

#include "info.h"
#include "license.h"

const wxChar* about2_license = about_license;
const wxChar* about2_name = wxT("Sudoku Solver");
const wxChar* about2_version = wxT("2.1");
const wxChar* about2_copyright = wxT(" (C) 2007 Thomas Gl‰ﬂle");
const wxChar* about2_developer = wxT("Thomas Gl‰ﬂle");
const wxChar* about2_description = about_info;
#define about2_icon GetIconResource(wxT("icon1.xpm"))

#endif