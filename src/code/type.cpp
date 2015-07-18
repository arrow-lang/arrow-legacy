// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>
#include "arrow/code/type.hpp"

namespace arrow {
namespace code {

// Name
// -----------------------------------------------------------------------------

std::string TypeSizedInteger::name() const {
  std::stringstream stream;
  if (!is_signed) stream << "u";
  stream << "int";
  stream << bits;
  return stream.str();
}

std::string TypeTuple::name() const {
  std::stringstream stream;
  stream << "(";
  unsigned idx;
  for (idx = 0; idx < elements.size(); ++idx) {
    if (idx > 0) {
      stream << ", ";
    }

    stream << elements.at(idx)->name();
  }
  if (idx == 1) {
    stream << ",";
  }
  stream << ")";
  return stream.str();
}

std::string TypeParameter::name() const {
  std::stringstream stream;
  if (keyword.size() > 0) stream << keyword << ": ";
  stream << type->name();
  return stream.str();
}

std::string TypeFunction::name() const {
  std::stringstream stream;
  stream << "(";
  unsigned idx;
  for (idx = 0; idx < parameters.size(); ++idx) {
    if (idx > 0) {
      stream << ", ";
    }

    stream << parameters.at(idx)->name();
  }
  stream << ")";
  stream << " -> ";
  stream << result->name();
  return stream.str();
}

// Equals
// -----------------------------------------------------------------------------

bool TypeSizedInteger::equals(Type& other) const {
  // Check that we are comparing against a sized-integer
  if (!Type::equals(other)) return false;
  auto other_si = dynamic_cast<TypeSizedInteger&>(other);

  // Equal if 'bits' and 'signed' is equal
  return bits == other_si.bits && is_signed == other_si.is_signed;
}

bool TypeTuple::equals(Type& other) const {
  // Check that we are comparing against a tuple
  if (!Type::equals(other)) return false;
  auto other_tu = dynamic_cast<TypeTuple&>(other);

  // Inequal if the number of elements are different
  if (elements.size() != other_tu.elements.size()) return false;

  // Equal if /all/ elements are equal
  for (unsigned idx = 0; idx < elements.size(); ++idx) {
    if (!elements.at(idx)->equals(*other_tu.elements.at(idx))) {
      return false;
    }
  }

  return true;
}

bool TypeParameter::equals(Type& other) const {
  // Check that we are comparing against a param
  if (!Type::equals(other)) return false;
  auto other_p = dynamic_cast<TypeParameter&>(other);

  // Equal if the type's are eqiuvalent. The name doesn't come into play
  // during type equality
  return type->equals(*other_p.type);
}

bool TypeFunction::equals(Type& other) const {
  // Check that we are comparing against a function
  if (!Type::equals(other)) return false;
  auto other_ft = dynamic_cast<TypeFunction&>(other);

  // Inequal if the number of parameters are different
  if (parameters.size() != other_ft.parameters.size()) return false;

  // Inequal if the result types are different
  if (!result->equals(*other_ft.result)) return false;

  // Equal if /all/ parameters are equal
  for (unsigned idx = 0; idx < parameters.size(); ++idx) {
    if (!parameters.at(idx)->equals(*other_ft.parameters.at(idx))) {
      return false;
    }
  }

  return true;
}

// Handle
// -----------------------------------------------------------------------------

LLVMTypeRef TypeBoolean::handle() {
  return LLVMInt1Type();
}

LLVMTypeRef TypeFloat::handle() {
  return LLVMDoubleType();
}

LLVMTypeRef TypeByte::handle() {
  return LLVMIntType(8);
}

LLVMTypeRef TypeInteger::handle() {
  // TODO(mehcode): Arbitrary percision integer type
  return LLVMIntType(128);
}

LLVMTypeRef TypeSizedInteger::handle() {
  return LLVMIntType(bits);
}

LLVMTypeRef TypeTuple::handle() {
  if (!_handle) {
    std::vector<LLVMTypeRef> element_types;
    element_types.reserve(elements.size());
    for (auto& element : elements) {
      element_types.push_back(element->handle());
    }

    _handle = LLVMStructType(
      element_types.data(), element_types.size(), false);
  }

  return _handle;
}

LLVMTypeRef TypeFunction::handle() {
  if (!_handle) {
    // Prepare the parameter type vector
    std::vector<LLVMTypeRef> parameter_handles;
    for (auto& param : parameters) {
      parameter_handles.push_back(param->type->handle());
    }

    // Prepare the result type
    auto result_handle = result->handle();

    // Construct the LLVM type
    _handle = LLVMFunctionType(
      result_handle, parameter_handles.data(), parameter_handles.size(),
      false);
  }

  return LLVMPointerType(_handle, 0);
}

LLVMTypeRef TypeParameter::handle() {
  return type->handle();
}

// Is Unknown
// -----------------------------------------------------------------------------

bool TypeTuple::is_unknown() const {
  for (auto& e : elements) {
    if (e->is_unknown()) return true;
  }

  return false;
}

bool TypeFunction::is_unknown() const {
  if (result->is_unknown()) return true;

  for (auto& p : parameters) {
    if (p->is_unknown()) return true;
  }

  return false;
}

bool TypeParameter::is_unknown() const {
  return type->is_unknown();
}

}  // namespace code
}  // namespace arrow
