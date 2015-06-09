// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>
#include <stdexcept>
#include <vector>
#include "arrow/ast.hpp"
#include "arrow/builder.hpp"
#include "arrow/code/type.hpp"
#include "arrow/llvm.hpp"

namespace code = arrow::code;

#define IMPL(N) \
  code::N::~N() noexcept { }

IMPL(Type)
IMPL(IntegerType)
IMPL(StringType)
IMPL(FloatType)
IMPL(BooleanType)
IMPL(FunctionType)
IMPL(PointerType)
// IMPL(StructureMember)
IMPL(StructureType)

code::IntegerType::IntegerType(ast::Node* context, code::Scope* scope, unsigned bits, bool is_signed)
  : Type(context, scope), bits(bits), _is_signed(is_signed) {
}

code::FloatType::FloatType(ast::Node* context, code::Scope* scope, unsigned bits)
  : Type(context, scope), bits(bits) {
}

code::FunctionType::FunctionType(ast::Node* context, code::Scope* scope, std::shared_ptr<code::Type> result)
  : Type(context, scope), result(result), parameters{} {
}

code::PointerType::PointerType(
  ast::Node* context,
  code::Scope* scope,
  std::shared_ptr<code::Type> pointee,
  bool _mutable
)
  : Type(context, scope), pointee(pointee), _mutable(_mutable) {
}

code::StructureType::StructureType(
  ast::Node* context,
  code::Scope* scope,
  const std::string& name
)
  : Type(context, scope), _name(name), _handle(nullptr) {
}

// code::StructureMember::StructureMember(
//   ast::Node* context,
//   code::Scope* scope,
//   const std::string& name,
//   std::shared_ptr<code::Type> type
// )
//   : Item(context, scope), name(name), type(type) {
// }

LLVMTypeRef code::IntegerType::handle(Generator& g) noexcept {
  return LLVMIntType(bits);
}

LLVMTypeRef code::BooleanType::handle(Generator& g) noexcept {
  return LLVMInt1Type();
}

LLVMTypeRef code::FloatType::handle(Generator& g) noexcept {
  switch (bits) {
    case 32:
      return LLVMFloatType();

    case 64:
      return LLVMDoubleType();

    default:
      throw std::runtime_error("invalid bit size for a floating-point type");
  }
}

LLVMTypeRef code::FunctionType::handle(Generator& g) noexcept {
  // Determine the result type (either void or declared)
  // TODO(mehcode): Full body deduction should come eventually
  auto res = result ? result->handle(g) : LLVMVoidType();

  // Collect the type handles for all paramters
  std::vector<LLVMTypeRef> params;
  params.reserve(parameters.size());
  for (auto& p : parameters) {
    params.push_back(p->handle(g));
  }

  return LLVMFunctionType(res, params.data(), params.size(), false);
}

LLVMTypeRef code::StringType::handle(Generator& g) noexcept {
  return LLVMPointerType(LLVMIntType(8), 0);
}

LLVMTypeRef code::PointerType::handle(Generator& g) noexcept {
  return LLVMPointerType(pointee->handle(g), 0);
}

LLVMTypeRef code::StructureType::handle(Generator& g) noexcept {
  if (_handle == nullptr) {
    _handle = LLVMStructCreateNamed(LLVMGetGlobalContext(), _name.c_str());

    std::vector<LLVMTypeRef> elements;
    for (auto& member : static_cast<ast::Structure*>(context)->members) {
      // Build the type for each member
      auto type = arrow::Builder{g, *scope}.build_type(*member->type);
      if (!type) { return nullptr; }

      elements.push_back(type->handle(g));
    }

    LLVMStructSetBody(_handle,
      elements.data(), elements.size(), false);
  }

  return _handle;
}

std::string code::IntegerType::name() const noexcept {
  std::stringstream stream;
  if (!_is_signed) stream << "u";
  stream << "int";
  stream << bits;
  return stream.str();
}

std::string code::FloatType::name() const noexcept {
  std::stringstream stream;
  stream << "float";
  stream << bits;
  return stream.str();
}

std::string code::FunctionType::name() const noexcept {
  // TODO
  return "FUNCTION TYPE";
}

std::string code::PointerType::name() const noexcept {
  std::stringstream stream;
  stream << "*";
  if (is_mutable()) stream << "mutable ";
  stream << pointee->name();
  return stream.str();
}

bool code::Type::equals(code::Type&) const noexcept {
  return false;
}

bool code::IntegerType::equals(code::Type& other) const noexcept {
  if (other.is<code::IntegerType>()) {
    auto& other_int = other.as<code::IntegerType>();
    return other_int.bits == bits && (other_int._is_signed == _is_signed);
  }

  return false;
}

bool code::FloatType::equals(code::Type& other) const noexcept {
  if (other.is<code::FloatType>()) {
    auto& other_float = other.as<code::FloatType>();
    return other_float.bits == bits;
  }

  return false;
}

bool code::BooleanType::equals(code::Type& other) const noexcept {
  return other.is<code::BooleanType>();
}

bool code::StringType::equals(code::Type& other) const noexcept {
  return other.is<code::StringType>();
}

bool code::PointerType::equals(code::Type& other) const noexcept {
  if (other.is<code::PointerType>()) {
    auto& other_ptr = other.as<code::PointerType>();
    return other_ptr.pointee->equals(*pointee) && (other_ptr.is_mutable() == is_mutable());
  }

  return false;
}

bool code::StructureType::equals(code::Type& other) const noexcept {
  if (other.is<code::StructureType>()) {
    auto& other_st = other.as<code::StructureType>();
    return other_st._name == _name;
  }

  return false;
}
