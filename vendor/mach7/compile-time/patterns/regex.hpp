///
/// \file regex.hpp
///
/// This file defines regular expression pattern.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"
#include <regex>
#include <string>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// RegEx pattern of 0 arguments
struct regex0 : std::regex
{
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    regex0(const char* re) : std::regex(re) {}
    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept { std::cmatch m; return regex_match(s,m,*this); }
};

//------------------------------------------------------------------------------

/// RegEx pattern of 1 arguments
template <typename P1>
struct regex1 : std::regex
{
    static_assert(is_pattern<P1>::value,    "Argument P1 of a regex-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    regex1(const char* re, const P1&  p1) noexcept : std::regex(re), m_p1(          p1 ) {}
    regex1(const char* re,       P1&& p1) noexcept : std::regex(re), m_p1(std::move(p1)) {}
    regex1(const regex1&  src) noexcept : std::regex(src), m_p1(          src.m_p1 ) {} ///< Copy constructor    
    regex1(      regex1&& src) noexcept : std::regex(src), m_p1(std::move(src.m_p1)) {} ///< Move constructor
    regex1& operator=(const regex1&); ///< Assignment is not allowed for this class

    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept
    {
        std::cmatch m; 

        if (regex_match(s,m,*this))
        {
            XTL_ASSERT(m.size() > 1);   // There should be enough capture groups for each of the pattern arguments
            typename P1::template accepted_type_for<std::string>::type m1;
            std::stringstream ss(m[1]); // m[0] is the entire expression
            return (ss >> m1) && m_p1(m1);
        }

        return false;
    }
    P1 m_p1;
};

//------------------------------------------------------------------------------

/// RegEx pattern of 2 arguments
template <typename P1, typename P2>
struct regex2 : std::regex
{
    static_assert(is_pattern<P1>::value,    "Argument P1 of a regex-pattern must be a pattern");
    static_assert(is_pattern<P2>::value,    "Argument P2 of a regex-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    regex2(const char* re, const P1&  p1, const P2&  p2) noexcept : std::regex(re), m_p1(          p1 ), m_p2(          p2 ) {}
    regex2(const char* re,       P1&& p1, const P2&  p2) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(          p2 ) {}
    regex2(const char* re, const P1&  p1,       P2&& p2) noexcept : std::regex(re), m_p1(          p1 ), m_p2(std::move(p2)) {}
    regex2(const char* re,       P1&& p1,       P2&& p2) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    regex2(const regex2&  src) noexcept : std::regex(src), m_p1(          src.m_p1 ), m_p2(          src.m_p2 ) {} ///< Copy constructor    
    regex2(      regex2&& src) noexcept : std::regex(src), m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)) {} ///< Move constructor
    regex2& operator=(const regex2&); ///< Assignment is not allowed for this class

    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept
    {
        std::cmatch m; 

        if (regex_match(s,m,*this))
        {
            XTL_ASSERT(m.size() > 2);   // There should be enough capture groups for each of the pattern arguments
            typename P1::template accepted_type_for<std::string>::type m1;
            std::stringstream ss1(m[1]); // m[0] is the entire expression

            if ((ss1 >> m1) && m_p1(m1))
            {
                typename P2::template accepted_type_for<std::string>::type m2;
                std::stringstream ss2(m[2]); // FIX: Was unable to reuse ss1 for some rason
                return (ss2 >> m2) && m_p2(m2);
            }
        }

        return false;
    }
    P1 m_p1;
    P2 m_p2;
};

//------------------------------------------------------------------------------

/// RegEx pattern of 3 arguments
template <typename P1, typename P2, typename P3>
struct regex3 : std::regex
{
    static_assert(is_pattern<P1>::value,    "Argument P1 of a regex-pattern must be a pattern");
    static_assert(is_pattern<P2>::value,    "Argument P2 of a regex-pattern must be a pattern");
    static_assert(is_pattern<P3>::value,    "Argument P3 of a regex-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    regex3(const char* re, const P1&  p1, const P2&  p2, const P3&  p3) noexcept : std::regex(re), m_p1(          p1 ), m_p2(          p2 ), m_p3(          p3 ) {}
    regex3(const char* re,       P1&& p1, const P2&  p2, const P3&  p3) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(          p2 ), m_p3(          p3 ) {}
    regex3(const char* re, const P1&  p1,       P2&& p2, const P3&  p3) noexcept : std::regex(re), m_p1(          p1 ), m_p2(std::move(p2)), m_p3(          p3 ) {}
    regex3(const char* re,       P1&& p1,       P2&& p2, const P3&  p3) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(          p3 ) {}
    regex3(const char* re, const P1&  p1, const P2&  p2,       P3&& p3) noexcept : std::regex(re), m_p1(          p1 ), m_p2(          p2 ), m_p3(std::move(p3)) {}
    regex3(const char* re,       P1&& p1, const P2&  p2,       P3&& p3) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(          p2 ), m_p3(std::move(p3)) {}
    regex3(const char* re, const P1&  p1,       P2&& p2,       P3&& p3) noexcept : std::regex(re), m_p1(          p1 ), m_p2(std::move(p2)), m_p3(std::move(p3)) {}
    regex3(const char* re,       P1&& p1,       P2&& p2,       P3&& p3) noexcept : std::regex(re), m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)) {}
    regex3(const regex3&  src) noexcept : std::regex(src), m_p1(          src.m_p1 ), m_p2(          src.m_p2 ), m_p3(          src.m_p3 ) {} ///< Copy constructor    
    regex3(      regex3&& src) noexcept : std::regex(src), m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)), m_p3(std::move(src.m_p3)) {} ///< Move constructor
    regex3& operator=(const regex3&); ///< Assignment is not allowed for this class

    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept
    {
        std::cmatch m; 

        if (regex_match(s,m,*this))
        {
            XTL_ASSERT(m.size() > 3);   // There should be enough capture groups for each of the pattern arguments
            typename P1::template accepted_type_for<std::string>::type m1;
            std::stringstream ss1(m[1]); // m[0] is the entire expression

            if ((ss1 >> m1) && m_p1(m1))
            {
                typename P2::template accepted_type_for<std::string>::type m2;
                std::stringstream ss2(m[2]); // FIX: Was unable to reuse ss1 for some rason

                if ((ss2 >> m2) && m_p2(m2))
                {
                    typename P3::template accepted_type_for<std::string>::type m3;
                    std::stringstream ss3(m[3]); // FIX: Was unable to reuse ss1 for some rason
                    return (ss3 >> m3) && m_p3(m3);
                }
            }
        }

        return false;
    }
    P1 m_p1;
    P2 m_p2;
    P3 m_p3;
};

//------------------------------------------------------------------------------

/// A 0-argument version of a regular-expression-pattern constructor.
inline regex0 rex(const char* re) noexcept { return regex0(re); }

/// A 1-argument version of a regular-expression-pattern constructor.
template <typename P1>
inline auto rex(const char* re, P1&& p1) noexcept -> XTL_RETURN
(
    regex1<
        typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
    >(
        re, 
        filter(std::forward<P1>(p1))
     )
)

/// A 2-argument version of a regular-expression-pattern constructor.
template <typename P1, typename P2>
inline auto rex(const char* re, P1&& p1, P2&& p2) noexcept -> XTL_RETURN
(
    regex2<
        typename underlying<decltype(filter(std::forward<P1>(p1)))>::type,
        typename underlying<decltype(filter(std::forward<P2>(p2)))>::type
    >(
        re, 
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2))
     )
)

/// A 3-argument version of a regular-expression-pattern constructor.
template <typename P1, typename P2, typename P3>
inline auto rex(const char* re, P1&& p1, P2&& p2, P3&& p3) noexcept -> XTL_RETURN
(
    regex3<
        typename underlying<decltype(filter(std::forward<P1>(p1)))>::type,
        typename underlying<decltype(filter(std::forward<P2>(p2)))>::type,
        typename underlying<decltype(filter(std::forward<P3>(p3)))>::type
    >(
        re, 
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3))
     )
)

//------------------------------------------------------------------------------

template <>                                      struct is_pattern_<regex0>           { static const bool value = true; };
template <typename P1>                           struct is_pattern_<regex1<P1>>       { static const bool value = true; };
template <typename P1, typename P2>              struct is_pattern_<regex2<P1,P2>>    { static const bool value = true; };
template <typename P1, typename P2, typename P3> struct is_pattern_<regex3<P1,P2,P3>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch
