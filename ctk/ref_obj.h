#ifndef __CTK__REFOBJ_H__INCLUDED__
#define __CTK__REFOBJ_H__INCLUDED__

#include "interlocked.h"

namespace ctk
{

/*
    ref_obj (referenced object)
*/

class ref_obj{
public:
    void add_ref() { ++refcount_; }
    void dec_ref() { if (!--refcount_) dispose(); }

    interlocked_t use_count() const { return refcount_.get(); }

protected:
    ref_obj() : refcount_(1) { }
    virtual ~ref_obj() { }

    virtual void dispose() { delete this; }

private:
    interlocked refcount_;
};


}   // ns ctk


    // boost::intrusive_ptr<ctk::ref_obj>

inline void intrusive_ptr_add_ref(ctk::ref_obj* t)
{
    t -> add_ref();
}

inline void intrusive_ptr_release(ctk::ref_obj* t)
{
    t -> dec_ref();
}


#endif // __CTK__REFOBJ_H__INCLUDED__
