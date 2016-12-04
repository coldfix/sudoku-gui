#ifndef __CTK__INTERLOCKED_H__INCLUDED__
#define __CTK__INTERLOCKED_H__INCLUDED__

#ifdef WIN32
# include <winbase.h>   // InterlockedIncrement/-Decrement, etc
#else
#endif


namespace ctk
{

#ifdef WIN32
# define CTK_INTERLOCKED 1

    typedef long interlocked_t;

    inline interlocked_t interlocked_increment(
        volatile interlocked_t& v)
    {
        return InterlockedIncrement(&v);
    }

    inline interlocked_t interlocked_decrement(
        volatile interlocked_t& v)
    {
        return InterlockedDecrement(&v);
    }

    inline interlocked_t interlocked_exchange(
        volatile interlocked_t& v,
        interlocked_t ex)
    {
        return InterlockedExchange(&v, ex);
    }

#else

    typedef long interlocked_t;

    inline interlocked_t interlocked_increment(
        volatile interlocked_t& v)
    {
        return ++v;
    }

    inline interlocked_t interlocked_decrement(
        volatile interlocked_t& v)
    {
        return --v;
    }

    inline interlocked_t interlocked_exchange(
        volatile interlocked_t& v,
        interlocked_t ex)
    {
        interlocked_t tmp = v;
        v = ex;
        return tmp;
    }
#endif


class interlocked
{
    volatile interlocked_t value_;
public:
    interlocked() : value_(0) { }

    interlocked(interlocked_t initial_value)
        : value_(initial_value)
    { }


    interlocked_t operator ++ () { return interlocked_increment(value_); }
    interlocked_t operator ++ (int) { return interlocked_increment(value_) - 1; }

    interlocked_t operator -- () { return interlocked_decrement(value_); }
    interlocked_t operator -- (int) { return interlocked_decrement(value_) + 1; }


    interlocked_t exchange(interlocked_t new_value) {
        return interlocked_exchange(value_, new_value); }


    interlocked_t get() const { return value_; }
};


class refcount
    : public interlocked
{
public:
    refcount() : interlocked(1) { }
    refcount(interlocked_t initial_value) : interlocked(initial_value) { }

};


} // ns ctk


#endif // include guard
