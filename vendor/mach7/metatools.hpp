//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file defines several utility functions used in Mach7
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include "config.hpp"
#include <cstddef>
#include <utility>
#include <type_traits>

//#include <boost/mpl/print.hpp>

//------------------------------------------------------------------------------

#if defined(_MSC_VER) && _MSC_VER < 1700 || defined(__GNUC__) && XTL_GCC_VERSION < 40500 && !defined(__clang__)
/// Visual C++ 2010 and earlier versions of GCC do not include declval
namespace std { template <typename T> typename std::add_rvalue_reference<T>::type declval(); }
#endif

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Helper meta-function to remove reference and const qualification from a type.
/// Used to get an underlying selector type of Match' argument.
template <typename T>
struct underlying : std::remove_const<typename std::remove_reference<T>::type> {};
//struct underlying : std::remove_const<typename std::remove_reference<typename boost::mpl::print<T>::type>::type> {};

/// For some reason the above definition results in amgibuity in at least MSVC, 
/// so we have to provide this specialization to make it work.
XTL_MSC_ONLY(template <typename R, typename A1> struct underlying<R(&)(A1)> { typedef R(type)(A1); };)

//------------------------------------------------------------------------------

//template <typename T> inline T&& identity(T&& t) noexcept { return std::forward<T>(t); } // FIX: This breaks constructor pattern for the case of 1 argument with enable_if ...
template <typename T> inline T& identity(T& t) noexcept { return t; }

//------------------------------------------------------------------------------

template <typename member_type>   struct member_traits; ///< Intentionally undefined.
template <typename C, typename R> struct member_traits<R (C::*)() const> { typedef C const class_type; typedef R result_type; };
template <typename C, typename R> struct member_traits<R (C::*)()      > { typedef C       class_type; typedef R result_type; };
template <typename C, typename R> struct member_traits<R  C::*         > { typedef C       class_type; typedef R result_type; };

//------------------------------------------------------------------------------

/// Helper metafunction to get the type of the first argument of a function type
template<typename T>              struct get_first_param;
template<typename R, typename P1> struct get_first_param<R(P1)> { typedef P1 type; };

//------------------------------------------------------------------------------

/// Helper meta-function used to obtain the first argument of a single-argument
/// template instantiation.
/// \note Default template does not have implementation by design to fail on 
///       inappropriate application.
template <typename T>                         struct get_param;
/// This is the actual implementation that grabs the first parameter for any 
/// single-argument template Q instantiation.
template <class U, template<class X> class Q> struct get_param<Q<U>> { typedef U type; };
/// This specialization is for uses of #bindings that employ layouts.
template <class U, size_t L, template<class X, size_t Y> class Q> struct get_param<Q<U,L>> { typedef U type; };
//template <class U, size_t L>                  struct get_param<bindings<U,L>> { typedef U type; };

//------------------------------------------------------------------------------

/// A class representing a set of locations of type T, indexed by a usually local
/// type UID that uniquely identifies the deferred constant. 
/// The class is used to implicitly introduce global variables in block
/// scopes, whose initializer will only be known later in the lexical scope. 
/// Initialization of such variables will happen before main() and thus accesses
/// to these locations from within any function that was called after main can
/// rely on the value of initializer to be known.
///
/// \note This class cannot be used to pass deferred constants to objects instantiated 
///       through the use of preallocated<> template below. The reason is that 
///       initialization of such objects happens before main and thus corresponding 
///       set<UID> may not have been initialized yet!
template <typename T>
struct deferred_constant
{
    /// Accessor to location UID of type T
    template <typename UID>      struct get { static T value; };
    /// A way to associate deferred initializer with the above location.
    /// \note This may happen later in the lexical scope with the net effect 
    ///       that the result of the association will be available in an earlier
    ///       access to the location through get<> without having to execute any
    ///       code at the program point where association has been made.
    template <typename UID, T V> struct set { static T* value_ptr; };
};

/// We let the location be default initialized first, while we will overwrite 
/// the default value a bit later in the initialization of set<>.
template <class T> 
template <typename UID> 
T deferred_constant<T>::get<UID>::value;

/// The trick that makes the above possible: instantiation of set will introduce
/// another static variable, whose dynamic initialization (before main) will 
/// force the initializer to be properly set.
template <class T> 
template <typename UID, T V> 
T* deferred_constant<T>::set<UID,V>::value_ptr = &(deferred_constant<T>::get<UID>::value = V);

//------------------------------------------------------------------------------

/// This class is an alternative to static variables inside functions, allowing 
/// it to be preallocated and thus avoid if in the function body. Parameter UID is
/// Allocation Identifier - a usually local type that uniquely identifies allocation.
/// The disadvantage of using this class might be worse locality as the static 
/// variable inside this class, even though preallocated will most likely be 
/// elsewhere.
template <typename T, typename UID>
struct preallocated
{
    static T value;
};

template <typename T, typename UID>
T preallocated<T,UID>::value;

//------------------------------------------------------------------------------

/// Helper function to help disambiguate a unary version of a given function when 
/// overloads with different arity are available.
/// All of the members we work with so far through #bindings are unary:
/// they are either unary function, nullary member function (implicit argument 
/// this makes them unary effectively) or a data member (which can be treated
/// in the same way as nullary member function).
template <typename R, typename A1> constexpr R (    * unary(R (    *f)(A1)      ) noexcept)(A1)     { return f; }
template <typename R, typename A1> constexpr R (A1::* unary(R  A1::*f           ) noexcept)         { return f; }
template <typename R, typename A1> constexpr R (A1::* unary(R (A1::*f)(  )      ) noexcept)()       { return f; }
template <typename R, typename A1> constexpr R (A1::* unary(R (A1::*f)(  ) const) noexcept)() const { return f; }

//------------------------------------------------------------------------------

#define XTL_DEBUG_APPLY_MEMBER(what, c, f) //XTL_DEBUG_ONLY(std::clog << "\nApplying " what << c << " of type " << typeid(*c).name() << std::endl)

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (T::*method)() const) noexcept_when(noexcept_of((c->*method)()))
{
    XTL_DEBUG_APPLY_MEMBER("const member function to const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)() const) noexcept_when(noexcept_of((c->*method)()))
{
    XTL_DEBUG_APPLY_MEMBER("const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)()      ) noexcept_when(noexcept_of((c->*method)()))
{
    XTL_DEBUG_APPLY_MEMBER("non-const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline const R& apply_member(const C* c, R T::*field) noexcept
{
    XTL_DEBUG_APPLY_MEMBER("data member to const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline       R& apply_member(      C* c, R T::*field) noexcept
{
    XTL_DEBUG_APPLY_MEMBER("data member to non-const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T*)) noexcept_when(noexcept_of((*func)(c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T*)) noexcept_when(noexcept_of((*func)(c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T*)) noexcept_when(noexcept_of((*func)(c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T&)) noexcept_when(noexcept_of((*func)(*c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T&)) noexcept_when(noexcept_of((*func)(*c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T&)) noexcept_when(noexcept_of((*func)(*c)))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

/// We need this extra indirection to be able to intercept when we are trying to
/// match a meta variable _ of type wildcard, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
inline bool apply_expression(const E& e, const C* c, M m) noexcept_when(noexcept_of(e(apply_member(c, m))))
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

template <typename E, typename C, typename M>
inline bool apply_expression(const E& e,       C* c, M m) noexcept_when(noexcept_of(e(apply_member(c, m))))
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

//------------------------------------------------------------------------------

} // of namespace mch
