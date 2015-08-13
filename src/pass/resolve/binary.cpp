// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/match.hpp"
#include "arrow/log.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::do_binary(
  ast::Binary& x,
  std::function<Ref<code::Type>(
    Ref<code::Type> lhs,
    Ref<code::Type> rhs)> cb
) {
  // Resolve the type of each operand in the expression
  auto lhs = Resolve(_scope).run(*x.lhs);
  auto rhs = Resolve(_scope).run(*x.rhs);
  if (!lhs || !rhs) return;
  if (lhs->is_unknown() || rhs->is_unknown()) {
    _stack.push_front(new code::TypeUnknown());
    return;
  }

  // Determine the resultant type
  auto result = cb(lhs, rhs);
  if (!result) {
    // Report operator not-found
    Log::get().error(
      x.span, "cannot apply binary operator `%s` to types '%s' and '%s'",
      x.punctuator(),
      lhs->name().c_str(),
      rhs->name().c_str());

    return;
  }

  _stack.push_front(result);
}

void Resolve::visit_add(ast::Add& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} + {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    // TODO: string + string

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_sub(ast::Sub& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} - {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_mul(ast::Mul& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} - {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    // TODO: string * {int} / {int} * string (?)

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_div(ast::Div& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} - {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_mod(ast::Mod& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int} - {int}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_eq(ast::EqualTo& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} == {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool == bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_ne(ast::NotEqualTo& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} != {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool != bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_lt(ast::LessThan& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} < {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool < bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_le(ast::LessThanOrEqualTo& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} <= {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool <= bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_ge(ast::GreaterThanOrEqualTo& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} >= {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool >= bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_gt(ast::GreaterThan& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int,float} > {int,float}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>() ||
         lhs.is<code::TypeFloat>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>() ||
         rhs.is<code::TypeFloat>())) {
      // Attempt to find the "common" type (for the result)
      if (code::intersect_all({lhs, rhs})) {
        // The result type is <bool>
        return Ref<code::Type>(new code::TypeBoolean());
      }
    }

    Match(*lhs, *rhs) {
      // bool > bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_bit_and(ast::BitAnd& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int} & {int}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    Match(*lhs, *rhs) {
      // bool & bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_bit_xor(ast::BitXor& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int} ^ {int}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    Match(*lhs, *rhs) {
      // bool ^ bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_bit_or(ast::BitOr& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    // General match: {int} | {int}
    if ((lhs.is<code::TypeInteger>() ||
         lhs.is<code::TypeIntegerLiteral>() ||
         lhs.is<code::TypeSizedInteger>()) && (
         rhs.is<code::TypeInteger>() ||
         rhs.is<code::TypeIntegerLiteral>() ||
         rhs.is<code::TypeSizedInteger>())) {
      // Attempt to find the "common" type (for the result)
      return code::intersect_all({lhs, rhs});
    }

    Match(*lhs, *rhs) {
      // bool | bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_and(ast::And& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    Match(*lhs, *rhs) {
      // bool AND bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

void Resolve::visit_or(ast::Or& x) {
  do_binary(x, [this](Ref<code::Type> lhs, Ref<code::Type> rhs) {
    Match(*lhs, *rhs) {
      // bool OR bool
      Case(code::TypeBoolean& a, code::TypeBoolean& b) {
        XTL_UNUSED(a);
        XTL_UNUSED(b);

        return lhs;
      }
    } EndMatch;

    return Ref<code::Type>(nullptr);
  });
}

}  // namespace pass
}  // namespace arrow
