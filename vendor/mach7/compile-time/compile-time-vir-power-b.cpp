///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#include <iostream>
#include "virpat.hpp"
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

typedef std::pair<double,int> arg_type;

//------------------------------------------------------------------------------

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
double power_opt(double x, int n)
{
	if (n == 0) return 1.0;
	if (n == 1) return x;
	return 
        n%2 == 0
            ?   sqr(power_opt(x,n/2)) 
            : x*sqr(power_opt(x,n/2));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

extern double power2(const double, const object_of<int>&);

inline double power2(const arg_type a) { return power2(a.first,object_of<int>(a.second)); }

//------------------------------------------------------------------------------

value_of<int> val0 = value_of<int>(0);
value_of<int> val1 = value_of<int>(1);

XTL_TIMED_FUNC_BEGIN
double power2(const double x, const object_of<int>& n)
{
    if (val0.matches(n))     return 1.0;
    if (val1.matches(n))     return x;

    var_of<int>    m;
    p_times_c<int> x2(m,2);
    
    if (x2.matches(n))       return sqr(power2(x,object_of<int>(m)));

    p_plus_c<int>  p1(x2,1);

    if (p1.matches(n))       return x*sqr(power2(x,object_of<int>(m)));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------


int main()
{
    std::vector<arg_type> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = arg_type(1.0+1.0/double(1+rand()-RAND_MAX/2), rand() % 100);

    verdict v = get_timings1<double,arg_type,power2,power2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
