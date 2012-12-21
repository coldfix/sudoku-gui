#pragma once

/// global classes

template<class T>
class GArrayPointer{
public:
    typedef T valtype;
    typedef valtype* iterator;
    typedef const valtype* const_iterator;

    GArrayPointer()
        : pvals(0),len(0)                               { }
    GArrayPointer(valtype*v,int s)
        : pvals(v),len(s)                               { }
    GArrayPointer(const GArrayPointer&vb)
        : pvals(vb.pvals), len(vb.len)                  { }
    virtual ~GArrayPointer()                            { }

    int size() const                                    { return len; }

    iterator begin()                                    { return pvals; }
    const_iterator begin() const                        { return pvals; }
    iterator end()                                      { return pvals+size(); }
    const_iterator end() const                          { return pvals+size(); }
    operator iterator ()                                { return begin(); }
    operator const_iterator() const                     { return begin(); }

    valtype& back()                                     { return at(size()-1); }
    valtype back() const                                { return at(size()-1); }
    valtype& front()                                    { return at(0); }
    valtype front() const                               { return at(0); }
    valtype& operator*()                                { return front(); }
    valtype operator*() const                           { return front(); }

    valtype& at(int i)                                  { return pvals[i]; }
    valtype at(int i) const                             { return pvals[i]; }
    valtype& operator[](int i)                          { return at(i); }
    valtype operator[](int i) const                     { return at(i); }

    void clear()                                        { len = 0; }
    void erase(int i)                                   { at(i) = at(--len); }
    void erase(iterator pos)                            { --len; *pos = *end(); }
    void append(valtype val)                            { *end() = val; resize(size()+1); }
    void append(const GArrayPointer& pt){
        int l = size();
        resize( size()+pt.size() );
        for(const_iterator it=pt.begin();it!=pt.end();it++)
            at(l++) = *it; }

    void resize(int x){
        if(x>size())
            doresize(x);
        else
            len = x; }
    virtual void doresize(int x)                        { len = x; }

    void reassign(const GArrayPointer&vb)               { pvals = vb.pvals; len = vb.len; }
    void reassign(valtype*pv,int s)                     { pvals = pv; len = s; }

    void assign(const GArrayPointer&vb){
        resize(vb.size());
        for(int i=size();i--;)
            at(i) = vb.at(i);
    }
    void assign(int,valtype val)                        { resize(1); front() = val; }

    GArrayPointer& operator=(const GArrayPointer&vb)    { assign(vb); return *this; }

    iterator find(valtype val){
        iterator it;
        for(it = begin(); it!=end() ; it++){
            if(*it==val)
                return it; }
        return it; }
    const_iterator find(valtype val) const{
        const_iterator it;
        for(it = begin(); it!=end() ; it++){
            if(*it==val)
                return it; }
        return it; }

    bool equal(const GArrayPointer& pt) const{
        if(size()!=pt.size())
            return false;
        for(int i=size();i--;)
            if( at(i) != pt.at(i) )
                return false;
        return true; }
    bool operator==(const GArrayPointer&pt) const{
        return equal(pt); }
    bool operator!=(const GArrayPointer&pt) const{
        return ! equal(pt); }


    void appendunique(iterator b,iterator e){
        while(b!=e)
            appendunique(*b++); }
    void appendunique(valtype val){
        if(find(val)==end())
            append(val); }

    int rem( valtype val ){
        iterator it = find(val);
        if(it!=end()){
            erase(it);
            return 1; }
        return 0; }
    int rem(const GArrayPointer&vr){
        int count = 0;
        for(const_iterator it=vr.begin() ; it!=vr.end() ; it++ )
            if(rem(*it))
                ++count;
        return count; }
    int rem(const GArrayPointer va[], int nvec){
        int count = 0;
        while(nvec--)
            count += rem(va[nvec]);
        return count; }


    int remall( valtype val ){
        int count = 0;
        for(iterator it=find(val) ; it!=end(); ++count,it=find(val) )
            v.erase(it);
        return count; }
    int remall(const GArrayPointer&vr){
        int count = 0;
        for(const_iterator it=begin() ; it!=end() ; it++ )
            count += remall(*it);
        return count; }
    int remall(const GArrayPointer va[], int nvec){
        int count = 0;
        while(nvec--)
            count += remall(va[nvec]);
        return count; }

    static GArrayPointer* CreateArray(int num,int size){
        GArrayPointer* pt = new GArrayPointer[num];
        valtype* pvals = new valtype[num*size];
        for(;num--;)
            pt[num].reassign( GArrayPointer( pvals+num*size, 0 ) );
        return pt; }
    static void DestroyArray(GArrayPointer*pt){
        delete [] pt[0].begin();
        delete [] pt; }
    static void ClearArray(GArrayPointer*pt,int size){
        for(;size--;)
            pt[size].clear(); }
    static void CopyArray(GArrayPointer*pdest,const GArrayPointer*psrc,int num){
        for(; num--; )
            pdest[num] = psrc[num]; }

    static bool CmpArray(GArrayPointer*pt1,GArrayPointer*pt2,int num){
        for(;num--;)
            if(pt1[num]!=pt2[num])
                return false;
        return true; }

protected:
    valtype*pvals;
    int len;
};

template<class T>
class GDynArray : public GArrayPointer<T>{
    int capacity;
    static GDynArray sda;
public:
    GDynArray(int cap)
        : GArrayPointer(new valtype[cap],0),
        capacity(cap)                                           { }
    GDynArray(int cap,int val)
        : GArrayPointer(new valtype[cap],1),
        capacity(cap)                                           { pvals[0] = val; }
    GDynArray(const GArrayPointer&vb)
        : GArrayPointer(new valtype[vb.size()],vb.size()),
        capacity(vb.size())                                     { assign(vb); }
    GDynArray(const GArrayPointer&vb,int cap)
        : GArrayPointer(new valtype[cap],vb.size()),
        capacity(cap)                                           { assign(vb); }
    GDynArray(const GDynArray&vb)
        : GArrayPointer(new valtype[vb.capacity],vb.size()),
        capacity(vb.capacity)                                   { assign(vb); }
    ~GDynArray()                                                { delete [] pvals; }


    static const GDynArray& range(int min,int range){
        sda.clear();
        sda.resize(range);
        for(int i=0;i<range; i++)
            sda[i] = min+i;
        return sda; }

    static const GDynArray& full(int r){
        return range(1,r); }

    void shrink()                                               { setcap(size()); }
    void setcap(int cap){
        valtype* old = pvals;
        pvals = new valtype[capacity=cap];
        if(cap<len)
            len = cap;
        for(int i=len;i--;)     /* save buffer */
            at(i) = old[i];
        delete [] old; }

    GDynArray& operator=(const GArrayPointer&vb)                { assign(vb); return *this; }
    GDynArray& operator=(const GDynArray&vb)                    { assign(vb); return *this; }

    void doresize(int x){
        if(x>capacity)
            setcap(x);
        len = x; }
};
template<class T>
GDynArray<T> GDynArray<T>::sda(150);


/*
    todo...
*/

template<class T>
class GPointerArray : public GDynArray<T*>
{
public:
};
