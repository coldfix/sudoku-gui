#pragma once

#ifdef _DEBUG
#define USE_COUNT
#endif


#ifdef USE_COUNT


#define DEBUG_DECLARE_COUNT(name) namespace debug{ int dv_count##name; static wxString dv_name##name; };
#define DEBUG_DEFINE_COUNT(name) int debug::dv_count##name = 0; wxString debug::dv_name##name = wxT(#name);

#define DEBUG_INC_COUNT(name) ++debug::dv_count##name;
#define DEBUG_DEC_COUNT(name) --debug::dv_count##name;

#define DEBUG_CREATE_COUNT_VIEW(name) new DebugView( debug::dv_count##name, debug::dv_name##name );

#include <wx/frame.h>
class wxStaticText;

class DebugView : public wxFrame{
    DECLARE_CLASS( DebugView )
    DECLARE_EVENT_TABLE()
public:
	enum{ ID_NUM_CTRL = 51 };

	DebugView(int&,const wxString&);

	void OnNumUpdate(wxUpdateUIEvent&);

private:
	int& count;
	wxString name;
	wxStaticText* num;
};


#else
#define DEBUG_DECLARE_COUNT(name) ;
#define DEBUG_IMP_COUNT(name) ;
#define DEBUG_INC_COUNT(name) ;
#define DEBUG_DEC_COUNT(name) ;
#define DEBUG_CREATE_COUNT_VIEW(name) ;
#endif





#ifdef _DEBUG
#include <wx/msgdlg.h>	/* fuer debug */
#define DEBUG_MSG( bedingung, msg )		if(bedingung) wxMessageBox( msg, wxT("wxSudoku") );
#define DBG(expression)		expression
#else
#define DEBUG_MSG( bedingung, msg)		true;
#define DBG(expression)			class {};
#endif

