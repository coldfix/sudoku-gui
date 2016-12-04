/////////////////////////////////////////////////////////////////////////////
// Name:        Validators.cpp
// Purpose:     Validators
// Author:      Thomas Gl‰ﬂle
// Modified by:
// Created:     2007-06-01
// Copyright:   (c) Thomas Gl‰ﬂle
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "Validators.h"
#endif

#include <wx/wxprec.h>
#include "validators.h"

#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/filename.h>

#include <wx/checkbox.h>
#include <wx/tglbtn.h>
#include <wx/radiobut.h>



namespace wxtk
{

/*
    SpinValidator
*/

IMPLEMENT_DYNAMIC_CLASS(SpinValidator, wxValidator)

BEGIN_EVENT_TABLE(SpinValidator, wxValidator)
END_EVENT_TABLE()

SpinValidator::SpinValidator(SpinInfo *val){
    m_spinInfo = val ; }


SpinValidator::SpinValidator(const SpinValidator& val){
    Copy(val); }

bool SpinValidator::Copy(const SpinValidator& val){
    wxValidator::Copy(val);
    m_spinInfo = val.m_spinInfo;
    return TRUE; }

SpinValidator::~SpinValidator(){
}

// Called when the value in the window must be validated.
// This function can pop up an error message.
bool SpinValidator::Validate(wxWindow *parent){
    if( !CheckValidator() )
        return FALSE;
    wxSpinCtrl *control = (wxSpinCtrl *) m_validatorWindow ;

    // If window is disabled, simply return
    if ( !control->IsEnabled() )
        return TRUE;
    if ( control->GetValue() < control->GetMin() || control->GetValue() > control->GetMax() ) {
        wxString errormsg = wxString::Format(
                                    wxT("The entered number is out of range!\nIt must be between %i and %i"),
                                    control->GetMin(),
                                    control->GetMax() );
        wxMessageBox(errormsg, _("Validation conflict"),
                     wxOK | wxICON_EXCLAMATION, parent);
        return FALSE; }
    return TRUE; }

// Called to transfer data to the window
bool SpinValidator::TransferToWindow(void){
    if( !CheckValidator() )
        return FALSE;
    wxSpinCtrl* control = (wxSpinCtrl*) m_validatorWindow ;
    control->SetRange(m_spinInfo->min,m_spinInfo->max);
    control->SetValue(m_spinInfo->value);
    control->Refresh();
    return TRUE; }

// Called to transfer data to the window
bool SpinValidator::TransferFromWindow(void){
    if( !CheckValidator() )
        return FALSE;
    wxSpinCtrl* control = (wxSpinCtrl*) m_validatorWindow ;
    m_spinInfo->value = control->GetValue();
    m_spinInfo->min = control->GetMin();
    m_spinInfo->max = control->GetMax();
    return TRUE; }

// Checks that the validator is set up correctly
bool SpinValidator::CheckValidator() const {
    wxCHECK_MSG( m_validatorWindow, FALSE,
                 _T("No window associated with validator") );
    wxCHECK_MSG( m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) , FALSE,
                 _T("SpinValidator is only for wxSpinCtrl") );
    wxCHECK_MSG( (m_spinInfo), FALSE,
                 _T("No variable storage for validator") );
    return TRUE; }



/*
    RadioGroup
*/


/// (con-)destruction
RadioGroup::RadioGroup()
{
}
RadioGroup::~RadioGroup()
{
    Clear();
}

/// access

int RadioGroup::GetSelectedIndex() const{
    for(int i=GetCount();i--;)
        if(Item(i)->GetValue())
            return i;
    return -1; }

void RadioGroup::SetSelectedIndex(int i){
    Item(i)->SetValue(true); }


/*
    FileNameDropTarget
*/

wxString FileNameDropTarget::FileArrayToString(const wxArrayString&filenames,const wxChar* sep){
    wxString lastpath = wxFileName(filenames[0]).GetPath();
    wxString txt = filenames[0];
    for(int i=1;i<filenames.GetCount();i++){
        wxFileName fn = filenames[i];
        if(fn.GetPath()!=lastpath){
            lastpath = wxFileName(filenames[i]).GetPath();
            txt += sep + filenames[i];
        }else
            txt += sep + fn.GetFullName();
    }
    return txt;
}

FileNameDropTarget::FileNameDropTarget(wxTextCtrl* droptarget,bool allowmultiple)
{
    m_droptarget = droptarget;
    m_allow_multiple = allowmultiple;
}

bool FileNameDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames){
    if(filenames.GetCount()==1)
        m_droptarget->SetValue( filenames[0] );
    else if(! GetAllowMultiple() )
        return false;
    else
        m_droptarget->SetValue( FileArrayToString(filenames) );
    return true; }



/*

*/


IMPLEMENT_DYNAMIC_CLASS(Validator,wxValidator)

BEGIN_EVENT_TABLE(Validator, wxValidator)
END_EVENT_TABLE()


// Constructors
Validator::Validator()
    : m_deliverer(0)
{
}
Validator::Validator(Deliverer*deliverer)
    : m_deliverer(deliverer)
{
}

Validator::Validator(const Validator& val)
    : m_deliverer(val.m_deliverer)
{
    m_deliverer->add_ref();
    wxValidator::Copy(val);
}

// Destructor
Validator::~Validator()
{
    m_deliverer->dec_ref();
}

// Copies val to this object
bool Validator::Copy(const Validator& val){
    wxValidator::Copy(val);
    m_deliverer = val.m_deliverer;
    m_deliverer->add_ref();
    return TRUE; }

///

// Called when the value in the window must be validated.
// This function can pop up an error message.
bool Validator::Validate(wxWindow *parent){
    if( !CheckValidator() )
        return FALSE;
    return true; }

// Called to transfer data to the window
bool Validator::TransferToWindow(){
    if( !CheckValidator() )
        return FALSE;
    bool ret = false;
    if(m_validatorWindow->IsKindOf(CLASSINFO(wxRadioButton))){
        ret = m_deliverer->DeliverTo( (wxRadioButton*)GetWindow() );
    }else if(m_validatorWindow->IsKindOf(CLASSINFO(wxCheckBox))){
        ret = m_deliverer->DeliverTo( (wxCheckBox*)GetWindow() );
    }else if(m_validatorWindow->IsKindOf(CLASSINFO(wxToggleButton))){
        ret = m_deliverer->DeliverTo( (wxToggleButton*)GetWindow() );
    }
    return ret; }

// Called to transfer data to the window
bool Validator::TransferFromWindow(){
    if( !CheckValidator() )
        return FALSE;
    return m_deliverer->DeliverFrom( GetWindow() ); }

// Checks that the validator is set up correctly
bool Validator::CheckValidator() const {
    wxCHECK_MSG( m_validatorWindow, FALSE, _T("No window associated with validator") );
    wxCHECK_MSG( (m_deliverer), FALSE, _T("No deliverer for validator") );
    if(! m_deliverer->IsValidWnd( GetWindow() ) )
        return false;
    return TRUE; }


/*

*/

bool CheckDeliverer::IsValidWnd(wxWindow* wnd){
    wxCHECK_MSG( wnd->IsKindOf(CLASSINFO(wxRadioButton)) || wnd->IsKindOf(CLASSINFO(wxCheckBox)) || wnd->IsKindOf(CLASSINFO(wxToggleButton)),
                FALSE, _T("CheckDeliverer is only for wxRadioButton, wxCheckBox, wxToggleButton") );
    return true;
}

CheckDeliverer::CheckDeliverer(){
}
CheckDeliverer::~CheckDeliverer(){
}

bool CheckDeliverer::DeliverTo(wxWindow*wnd){
    if(!IsValidWnd(wnd))
        return false;
    bool val = GetSelected();
    if(wnd->IsKindOf(CLASSINFO(wxRadioButton))){
        ((wxRadioButton*)wnd)->SetValue(val);
    }else if(wnd->IsKindOf(CLASSINFO(wxCheckBox))){
        ((wxCheckBox*)wnd)->SetValue(val);
    }else if(wnd->IsKindOf(CLASSINFO(wxToggleButton))){
        ((wxToggleButton*)wnd)->SetValue(val);
    }else
        return false;
    return true; }

bool CheckDeliverer::DeliverFrom(wxWindow*wnd){
    if(!IsValidWnd(wnd))
        return false;
    bool val;
    if(wnd->IsKindOf(CLASSINFO(wxRadioButton))){
        val = ((wxRadioButton*)wnd)->GetValue();
    }else if(wnd->IsKindOf(CLASSINFO(wxCheckBox))){
        val = ((wxCheckBox*)wnd)->GetValue();
    }else if(wnd->IsKindOf(CLASSINFO(wxToggleButton))){
        val = ((wxToggleButton*)wnd)->GetValue();
    }else
        return false;
    SetSelected(val);
    return true; }


} // ns wxtk
