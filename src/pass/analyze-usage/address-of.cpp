// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

static bool check_item(ast::Node& context, code::Item& item) {
  Match(item) {
    Case(code::Slot& slot) {
      if (!slot.is_mutable) {
        Log::get().error(
          context.span,
          "cannot capture mutable address of the immutable variable '%s'",
          slot.name.c_str());
      }

      return true;
    } break;

    Case(code::ExternSlot& slot) {
      if (!slot.is_mutable) {
        Log::get().error(
          context.span,
          "cannot capture mutable address of the immutable variable '%s'",
          slot.name.c_str());
      }

      return true;
    } break;

    Case(code::Parameter& param) {
      if (!param.is_mutable) {
        Log::get().error(
          context.span,
          "cannot capture mutable address of the immutable parameter '%s'",
          param.name.c_str());
      }

      return true;
    } break;
  } EndMatch;

  return false;
}

void AnalyzeUsage::_require_mutable(ast::Node& operand) {
  // If we are attempting to get a mutable address - check to
  // ensure that we are asking for a mutable value (slot, member, parameter)
  Match(operand) {
    Case(ast::Path& path) {
      auto item = util::get_item(_scope, path);
      if (item) {
        if (check_item(operand, *item)) return;
      } else {
        auto op_item = util::get_item(_scope, *path.operand);
        if (op_item) {
          // Resolve the type of the operand
          auto op_type = Resolve(_scope).run(*path.operand);
          if (!op_type) return;

          Match(*op_type) {
            Case(code::TypeStructure& struct_) {
              auto mem = struct_.find_member(path.member);
              auto is_mutable = false;
              Match(*op_item) {
                Case(code::Slot& slot) {
                  is_mutable = slot.is_mutable;
                } break;

                Case(code::ExternSlot& slot) {
                  is_mutable = slot.is_mutable;
                } break;

                Case(code::Parameter& param) {
                  is_mutable = param.is_mutable;
                } break;
              } EndMatch;

              if (!is_mutable) {
                Log::get().error(
                  operand.span,
                  "cannot capture mutable address of "
                  "the immutable member '%s'",
                  path.member.c_str());

                return;
              } else {
                return;
              }
            } break;
          } EndMatch;
        }
      }
    } break;

    Case(ast::Identifier& ident) {
      auto item = util::get_item(_scope, ident);
      if (!item) return;
      if (check_item(operand, *item)) return;
    } break;
  } EndMatch;

  Log::get().error(
    operand.span,
    "cannot capture mutable address of an immutable value");
}

void AnalyzeUsage::visit_address_of(ast::AddressOf& x) {
  // Visit this normally (check for unresolved)
  Visitor::visit_address_of(x);
  if (Log::get().count("error") > 0) return;

  // If we are attempting to get a mutable address - check to
  // ensure that we are asking for a mutable value (slot, member, parameter)
  if (x.is_mutable) {
    _require_mutable(*x.operand);
  }
}

}  // namespace pass
}  // namespace arrow
