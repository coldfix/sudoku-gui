#ifndef __WXTK__VALIDATORS_H__INCLUDED__
#define __WXTK__VALIDATORS_H__INCLUDED__

#include <wx/validate.h>
#include <wx/dnd.h>

#include "../ctk/ref_obj.h"


    /// forward declarations

class wxBitmap;
class wxRadioButton;
class wxTextCtrl;


namespace wxtk
{

/*!
 * Validator for wxSpinCtrl
 */

struct SpinInfo{
    SpinInfo(int p_value=0,int p_min=0, int p_max=100){
        value = p_value;
        min = p_min;
        max = p_max; }
/*  SpinInfo(const SpinInfo&val){
        value = val.value;
        min = val.min;
        max = val.max; }*/

    int value,min,max;

    bool operator == ( const SpinInfo& spininfo ) const{
        return value == spininfo.value && min == spininfo.min && max == spininfo.max; }
};

class SpinValidator: public wxValidator
{
    DECLARE_DYNAMIC_CLASS(SpinValidator)
    DECLARE_EVENT_TABLE()

public:
    // Constructors
    SpinValidator(SpinInfo *val = NULL);
    SpinValidator(const SpinValidator& val);

    // Destructor
    ~SpinValidator();

    // Make a clone of this validator
    virtual wxObject *Clone() const { return new SpinValidator(*this); }

    // Copies val to this object
    bool Copy(const SpinValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);

    // Called to transfer data to the window
    virtual bool TransferToWindow();

    // Called to transfer data to the window
    virtual bool TransferFromWindow();

    SpinInfo* GetSpinInfo() { return m_spinInfo; }


protected:
    SpinInfo*   m_spinInfo;

    // Checks that the validator is set up correctly
    bool CheckValidator() const ;
};


/*
    class for managing several wxRadioButtons without validator
*/

class RadioGroup{
public:

    /// types & consts
    WX_DEFINE_ARRAY( wxRadioButton*, RadioButtonArray );

    /// construction
    RadioGroup();
    ~RadioGroup();

    /// access
    int GetCount() const                    { return m_radios.GetCount(); }
    wxRadioButton* GetRadio(int i) const    { return m_radios[i]; }
    wxRadioButton* Item(int i) const        { return GetRadio(i); }

    void Add(wxRadioButton* button)         { m_radios.Add(button); }
    void Remove(wxRadioButton* button)      { RemoveAt(Index(button)); }
    void RemoveAt(int i)                    { m_radios.RemoveAt(i); }
    int Index(wxRadioButton* button) const  { return m_radios.Index(button); }
    void Clear()                            { m_radios.Clear(); }

    RadioButtonArray* GetArray()            { return &m_radios; }

    int GetSelectedIndex() const;
    void SetSelectedIndex(int);

private:
    /// members
    RadioButtonArray m_radios;
};



/*!
 * Validator for any window type.. forwards requests to deliverer
 */

class Validator : public wxValidator{
    DECLARE_DYNAMIC_CLASS(Validator)
    DECLARE_EVENT_TABLE()
public:

    // types & consts
    class Deliverer : public ctk::ref_obj{
    public:
        virtual bool DeliverTo(wxWindow*)=0;
        virtual bool DeliverFrom(wxWindow*)=0;
        virtual bool IsValidWnd(wxWindow*)=0;
    };


    // Constructors
    Validator();
    Validator(Deliverer*);
    Validator(const Validator& val);

    // Destructor
    ~Validator();

    // Make a clone of this validator
    virtual wxObject *Clone() const { return new Validator(*this); }

    // Copies val to this object
    bool Copy(const Validator& val);

    ///

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);

    // Called to transfer data to the window
    virtual bool TransferToWindow();

    // Called to transfer data to the window
    virtual bool TransferFromWindow();

    //
    Deliverer* GetDeliverer()               { return m_deliverer; }
    void SetDeliverer(Deliverer* deliverer) { m_deliverer = deliverer; }

protected:
    // Checks that the validator is set up correctly
    bool CheckValidator() const;


    // Members
    Deliverer* m_deliverer;

};

/*

*/

class CheckDeliverer : public Validator::Deliverer{
public:
    CheckDeliverer();
    virtual ~CheckDeliverer();

    virtual bool DeliverTo(wxWindow*);
    virtual bool DeliverFrom(wxWindow*);

    virtual bool IsValidWnd(wxWindow*);

    virtual bool GetSelected()=0;
    virtual bool SetSelected(bool buttonstate)=0;
};

/**/
template<class T>
class ValueCheckDeliverer : public CheckDeliverer{
public:
    typedef T ValueType;

    ValueCheckDeliverer(ValueType* selectval,ValueType persval)
        : m_selectedValue(selectval), m_personalValue( persval)
    {
    }

    bool GetSelected(){
        return *GetSelectedValue()==GetPersonalValue(); }
    bool SetSelected(bool buttonstate){
        if(buttonstate)
            *GetSelectedValue() = GetPersonalValue();
        return true; }

    ValueType* GetSelectedValue()               { return m_selectedValue; }
    const ValueType& GetPersonalValue()         { return m_personalValue; }
    void SetPersonalValue(const ValueType&val)  { m_personalValue = val; }
protected:
    ValueType *m_selectedValue, m_personalValue;
};


//
template<class T,class I=int>
class IndexCheckDeliverer : public ValueCheckDeliverer<T>{
public:
    typedef I IndexType;
    using typename ValueCheckDeliverer<T>::ValueType;

    IndexCheckDeliverer(ValueType* selectval,ValueType persval,IndexType* selectidx,IndexType persidx)
        : ValueCheckDeliverer<T>(selectval,persval), m_selectedIndex(selectidx), m_personalIndex(persidx)
    {
    }

    bool GetSelected(){
        return *GetSelectedIndex()==GetPersonalIndex(); }
    bool SetSelected(bool buttonstate){
        if(buttonstate){
            *this->GetSelectedValue() = this->GetPersonalValue();
            *this->GetSelectedIndex() = this->GetPersonalIndex(); }
        return true; }

    IndexType* GetSelectedIndex()               { return m_selectedIndex; }
    const IndexType& GetPersonalIndex()         { return m_personalIndex; }
    void SetPersonalIndex(const IndexType&idx)  { m_personalIndex = idx; }
protected:
    IndexType *m_selectedIndex, m_personalIndex;

};

//
template<class T>
class FlagCheckDeliverer : public ValueCheckDeliverer<T>{
public:
    using typename ValueCheckDeliverer<T>::ValueType;
    FlagCheckDeliverer(ValueType* selectval,ValueType persval,ValueType mask)
        : ValueCheckDeliverer<T>(selectval,persval), m_mask(mask)
    {
    }

    bool GetSelected(){
        return (*this->GetSelectedValue()&GetMask())==this->GetPersonalValue(); }
    bool SetSelected(bool buttonstate){
        if(buttonstate)
            *this->GetSelectedValue() = (*this->GetSelectedValue()&~GetMask()) | this->GetPersonalValue();
        return true; }

    const ValueType& GetMask()          { return m_mask; }
    void SetMask(const ValueType&mask)  { m_mask = mask; }
protected:
    ValueType m_mask;
};

typedef ValueCheckDeliverer<int> IntCheckDeliverer;
typedef IndexCheckDeliverer<int> IntIndexCheckDeliverer;
typedef FlagCheckDeliverer<int> IntFlagCheckDeliverer;
typedef ValueCheckDeliverer<void*> PtrCheckDeliverer;
typedef ValueCheckDeliverer<void*> PtrIndexCheckDeliverer;


/*
 FileNameDropTarget
*/


class FileNameDropTarget : public wxFileDropTarget
{
public:
    static wxString FileArrayToString(const wxArrayString&filenames,const wxChar* sep=wxT(";"));

    FileNameDropTarget(wxTextCtrl* droptarget,bool allowmultiple);

    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

    void SetAllowMultiple(bool allowmultiple)       { m_allow_multiple = allowmultiple; }
    bool GetAllowMultiple()                         { return m_allow_multiple; }
private:
    wxTextCtrl* m_droptarget;
    bool m_allow_multiple;
};


} // ns wxtk

#endif // include guard
