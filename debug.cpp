#include <wx/wxprec.h>


#include "debug.h"

#ifdef USE_COUNT

#include <wx/stattext.h>
#include <wx/sizer.h>

IMPLEMENT_CLASS( DebugView, wxFrame )

BEGIN_EVENT_TABLE( DebugView, wxFrame )
    EVT_UPDATE_UI( ID_NUM_CTRL, DebugView::OnNumUpdate )
END_EVENT_TABLE()


DebugView::DebugView( int& refcount, const wxString& name ) : count(refcount){
	Create(0,wxID_ANY,name,wxPoint(0,0));
	wxBoxSizer* outersizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(outersizer);
	wxBoxSizer* innersizer = new wxBoxSizer(wxHORIZONTAL);
	outersizer->Add(innersizer,1,wxEXPAND|wxALL, 10);

	wxStaticText* ctrlname = new wxStaticText(this,wxID_STATIC,name);
	innersizer->Add(ctrlname, 0, wxALL, 5);
	innersizer->AddSpacer(5);
	num = new wxStaticText(this,ID_NUM_CTRL, wxT(""));
	innersizer->Add(num,0,wxALL,5);

	GetSizer()->SetSizeHints(this);
	SetBackgroundColour(*wxWHITE);
	Fit();
	Move(0,0);
	Show(true);
}


void DebugView::OnNumUpdate(wxUpdateUIEvent&){
	wxString str = wxString::Format( wxT("%i"), count );
	if(str!=num->GetLabel())
		num->SetLabel( str );
}


#endif