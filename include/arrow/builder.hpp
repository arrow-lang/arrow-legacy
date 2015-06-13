// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_BUILDER_H
#define ARROW_BUILDER_H 1

#include <stack>
#include "arrow/ast.hpp"
#include "arrow/code.hpp"
#include "arrow/log.hpp"

namespace arrow {

class Generator;

class Builder : public ast::Visitor {
 public:
  Builder(Generator& g, code::Scope& _scope);

  virtual ~Builder() noexcept;

  void build(
    ast::Node& node, code::Scope* scope = nullptr);

  std::shared_ptr<code::Type> build_type(
    ast::Node& node, code::Scope* scope = nullptr);

  std::shared_ptr<code::Item> build_scalar(
    ast::Node& node, code::Scope* scope = nullptr);

  template <typename T>
  std::shared_ptr<T> build_scalar_of(
    ast::Node& node, code::Scope* scope = nullptr
  ) {
    auto item_generic = build_scalar(node, scope);
    if (item_generic == nullptr) return nullptr;

    auto item = std::dynamic_pointer_cast<T>(item_generic);
    if (item == nullptr) {
      Log::get().error(node.span, "expected %s", typeid(T).name());
    }

    return item;
  }

  virtual void visit_id(ast::Identifier&);
  // virtual void visit_module(ast::Module&);
  virtual void visit_function(ast::Function&);
  // virtual void visit_parameter(ast::Parameter&);
  virtual void visit_call(ast::Call&);
  virtual void visit_slot(ast::Slot&);
  virtual void visit_break(ast::Break&);
  virtual void visit_continue(ast::Continue&);
  virtual void visit_return(ast::Return&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_bool(ast::Boolean&);
  virtual void visit_str(ast::String&);
  virtual void visit_promote(ast::Promote&);
  virtual void visit_negate_logical(ast::NegateLogical&);
  virtual void visit_negate_bit(ast::NegateBit&);
  virtual void visit_negate_numeric(ast::NegateNumeric&);
  virtual void visit_assign(ast::Assign&);
  virtual void visit_and(ast::And&);
  virtual void visit_or(ast::Or&);
  virtual void visit_eq(ast::EqualTo&);
  virtual void visit_ne(ast::NotEqualTo&);
  virtual void visit_lt(ast::LessThan&);
  virtual void visit_le(ast::LessThanOrEqualTo&);
  virtual void visit_ge(ast::GreaterThanOrEqualTo&);
  virtual void visit_gt(ast::GreaterThan&);
  virtual void visit_bit_and(ast::BitAnd&);
  virtual void visit_bit_xor(ast::BitXor&);
  virtual void visit_bit_or(ast::BitOr&);
  virtual void visit_add(ast::Add&);
  virtual void visit_sub(ast::Sub&);
  virtual void visit_mul(ast::Mul&);
  virtual void visit_div(ast::Div&);
  virtual void visit_mod(ast::Mod&);
  virtual void visit_cast(ast::Cast&);
  virtual void visit_select(ast::Select&);
  // virtual void visit_select_branch(ast::SelectBranch&);
  virtual void visit_loop(ast::Loop&);
  virtual void visit_pointer_type(ast::PointerType&);
  virtual void visit_address_of(ast::AddressOf&);
  virtual void visit_dereference(ast::Dereference&);
  virtual void visit_block(ast::Block&);
  virtual void visit_member(ast::Member&);

  // Ignored (but still sent here) node types
  virtual void visit_struct(ast::Structure&) { }
  virtual void visit_import(ast::Import&) { }
  virtual void visit_extern_function(ast::ExternalFunction&) { }

 private:
  friend class Generator;

  // Struct to track loop positions.
  struct LoopFrame {
    LLVMBasicBlockRef condition;
    LLVMBasicBlockRef merge;
  };

  Generator& _g;
  code::Scope& _scope;
  code::Scope* _cs;
  code::Module* _cm;
  code::Function* _cf;
  std::stack<std::shared_ptr<code::Item>> _stack;
  std::stack<LoopFrame> _loops;

  std::shared_ptr<code::Item> do_sequence(
    std::deque<std::shared_ptr<ast::Node>>&,
    code::Scope* scope);

  void do_arithmetic(
    ast::Binary& x,
    std::function<LLVMValueRef(
      std::shared_ptr<code::Value>, std::shared_ptr<code::Value>)> cb);

  void do_bitwise(
    ast::Binary& x,
    std::function<LLVMValueRef(
      std::shared_ptr<code::Value>, std::shared_ptr<code::Value>)> cb);

  void do_relational(
    ast::Binary& x,
    std::function<int(std::shared_ptr<code::Type>)> cb);

  void do_unary(
    ast::Unary& x,
    std::function<LLVMValueRef(std::shared_ptr<code::Value>)> cb);
};

}  // namespace arrow

#endif  // ARROW_BUILDER_H
