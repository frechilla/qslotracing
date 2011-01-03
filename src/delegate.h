#ifndef DELEGATE_H
#define DELEGATE_H

// have a look to http://www.codeproject.com/KB/cpp/CPPDelegate.aspx
// to understand what is goind on here

/// @brief creates a static delegate
/// points to a function which doesn't belong to any object
#define MakeDelegateStatic(function_pointer) \
    (GetDelegateFactory(function_pointer).Bind<function_pointer>())

/// @brief creates a standard delegate
/// points to a function which belong to an object
#define MakeDelegate(function_pointer, instancePtr) \
    (GetDelegateFactory(function_pointer).Bind<function_pointer>(instancePtr))

template<typename FuncSignature>
class Delegate;

struct NullDelegate {};

// 0 parameter version

template<typename R>
class Delegate<R ()>
{
public:
    static const int Arity = 0;
    typedef R ReturnType;

    Delegate()                    : func(0), obj(0) {}
    Delegate(NullDelegate)        : func(0), obj(0) {}
    Delegate(const Delegate& rhs) : func(rhs.func), obj(rhs.obj) {}
    ~Delegate() {}

    Delegate& operator=(NullDelegate)
        { obj = 0; func = 0; return *this; }
    Delegate& operator=(const Delegate& rhs)
        { obj = rhs.obj; func = rhs.func; return *this; }

    inline R operator()() const
    {
        return (*func)(obj);
    }

private:
    typedef const void* Delegate::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return func != 0 ? &Delegate::obj : 0; }
    inline bool operator!() const
        { return func == 0; }

private:
    typedef R (*FuncType)(const void*);
    Delegate(FuncType f, const void* o) : func(f), obj(o) {}

private:
    FuncType func;
    const void* obj;

    template<typename FR>
    friend class FreeDelegateFactory0;
    template<typename FR, class FT>
    friend class MemberDelegateFactory0;
    template<typename FR, class FT>
    friend class ConstMemberDelegateFactory0;
};

template<typename R>
void operator==(const Delegate<R ()>&,
                const Delegate<R ()>&);
template<typename R>
void operator!=(const Delegate<R ()>&,
                const Delegate<R ()>&);

template<typename R>
class FreeDelegateFactory0
{
private:
    template<R (*Func)()>
    static R Wrapper(const void*)
    {
        return (*Func)();
    }

public:
    template<R (*Func)()>
    inline static Delegate<R ()> Bind()
    {
        return Delegate<R ()>
            (&FreeDelegateFactory0::Wrapper<Func>, 0);
    }
};

template<typename R>
inline FreeDelegateFactory0<R>
GetDelegateFactory(R (*)())
{
    return FreeDelegateFactory0<R>();
}

template<typename R, class T>
class MemberDelegateFactory0
{
private:
    template<R (T::*Func)()>
    static R Wrapper(const void* o)
    {
        T* obj = const_cast<T*>(static_cast<const T*>(o));
        return (obj->*Func)();
    }

public:
    template<R (T::*Func)()>
    inline static Delegate<R ()> Bind(T* o)
    {
        return Delegate<R ()>
            (&MemberDelegateFactory0::Wrapper<Func>,
            static_cast<const void*>(o));
    }
};

template<typename R, class T>
inline MemberDelegateFactory0<R, T>
GetDelegateFactory(R (T::*)())
{
    return MemberDelegateFactory0<R, T>();
}

template<typename R, class T>
class ConstMemberDelegateFactory0
{
private:
    template<R (T::*Func)() const>
    static R Wrapper(const void* o)
    {
        const T* obj = static_cast<const T*>(o);
        return (obj->*Func)();
    }

public:
    template<R (T::*Func)() const>
    inline static Delegate<R ()> Bind(const T* o)
    {
        return Delegate<R ()>
            (&ConstMemberDelegateFactory0::Wrapper<Func>,
            static_cast<const void*>(o));
    }
};

template<typename R, class T>
inline ConstMemberDelegateFactory0<R, T>
GetDelegateFactory(R (T::*)() const)
{
    return ConstMemberDelegateFactory0<R, T>();
}

// 1 parameter version

template<typename R, typename P1>
class Delegate<R (P1)>
{
public:
    static const int Arity = 1;
    typedef R ReturnType;
    typedef P1 Param1Type;

    Delegate()                    : func(0), obj(0) {}
    Delegate(NullDelegate)        : func(0), obj(0) {}
    Delegate(const Delegate& rhs) : func(rhs.func), obj(rhs.obj) {}
    ~Delegate() {}

    Delegate& operator=(NullDelegate)
        { obj = 0; func = 0; return *this; }
    Delegate& operator=(const Delegate& rhs)
        { obj = rhs.obj; func = rhs.func; return *this; }

    inline R operator()(P1 a1) const
    {
        return (*func)(obj, a1);
    }

private:
    typedef const void* Delegate::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return func != 0 ? &Delegate::obj : 0; }
    inline bool operator!() const
        { return func == 0; }

private:
    typedef R (*FuncType)(const void*, P1);
    Delegate(FuncType f, const void* o) : func(f), obj(o) {}

private:
    FuncType func;
    const void* obj;

    template<typename FR, typename FP1>
    friend class FreeDelegateFactory1;
    template<typename FR, class FT, typename FP1>
    friend class MemberDelegateFactory1;
    template<typename FR, class FT, typename FP1>
    friend class ConstMemberDelegateFactory1;
};

template<typename R, typename P1>
void operator==(const Delegate<R (P1)>&,
                const Delegate<R (P1)>&);
template<typename R, typename P1>
void operator!=(const Delegate<R (P1)>&,
                const Delegate<R (P1)>&);

template<typename R, typename P1>
class FreeDelegateFactory1
{
private:
    template<R (*Func)(P1)>
    static R Wrapper(const void*, P1 a1)
    {
        return (*Func)(a1);
    }

public:
    template<R (*Func)(P1)>
    inline static Delegate<R (P1)> Bind()
    {
        return Delegate<R (P1)>
            (&FreeDelegateFactory1::Wrapper<Func>, 0);
    }
};

template<typename R, typename P1>
inline FreeDelegateFactory1<R, P1>
GetDelegateFactory(R (*)(P1))
{
    return FreeDelegateFactory1<R, P1>();
}

template<typename R, class T, typename P1>
class MemberDelegateFactory1
{
private:
    template<R (T::*Func)(P1)>
    static R Wrapper(const void* o, P1 a1)
    {
        T* obj = const_cast<T*>(static_cast<const T*>(o));
        return (obj->*Func)(a1);
    }

public:
    template<R (T::*Func)(P1)>
    inline static Delegate<R (P1)> Bind(T* o)
    {
        return Delegate<R (P1)>
            (&MemberDelegateFactory1::Wrapper<Func>,
            static_cast<const void*>(o));
    }
};

template<typename R, class T, typename P1>
inline MemberDelegateFactory1<R, T, P1>
GetDelegateFactory(R (T::*)(P1))
{
    return MemberDelegateFactory1<R, T, P1>();
}

template<typename R, class T, typename P1>
class ConstMemberDelegateFactory1
{
private:
    template<R (T::*Func)(P1) const>
    static R Wrapper(const void* o, P1 a1)
    {
        const T* obj = static_cast<const T*>(o);
        return (obj->*Func)(a1);
    }

public:
    template<R (T::*Func)(P1) const>
    inline static Delegate<R (P1)> Bind(const T* o)
    {
        return Delegate<R (P1)>
            (&ConstMemberDelegateFactory1::Wrapper<Func>,
            static_cast<const void*>(o));
    }
};

template<typename R, class T, typename P1>
inline ConstMemberDelegateFactory1<R, T, P1>
GetDelegateFactory(R (T::*)(P1) const)
{
    return ConstMemberDelegateFactory1<R, T, P1>();
}


#endif // DELEGATE_H
