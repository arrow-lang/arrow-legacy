///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#include <utility>

// Enable declarations in case clauses, which are disabled by default
#define XTL_CLAUSE_DECL 1

#include "match.hpp"                // Support for Match statement

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    virtual ~Shape() {}
};

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : center(c), radius(r) {}
    loc    center;
    double radius;
};

struct Square : Shape
{
    Square(const loc& c, const double& s) : upper_left(c), side(s) {}
    loc    upper_left;
    double side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}
    loc first;
    loc second;
    loc third;
};

//------------------------------------------------------------------------------

loc point_within(const Shape* shape)
{
    Match(shape)
    {
    Case(const Circle&   x) return x.center;
    Case(const Square&   x) return x.upper_left;
    Case(const Triangle& x) return x.first;
    Otherwise()             return loc(0,0);
    }
    EndMatch
}

//------------------------------------------------------------------------------

int main()
{
    point_within(new Triangle(loc(0,0),loc(1,0),loc(0,1)));
    point_within(new Square(loc(1,0),1));
    point_within(new Circle(loc(0,0),1));
}

//------------------------------------------------------------------------------
