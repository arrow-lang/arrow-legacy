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
      stream << ",";
    }

    stream << elements.at(idx)->name();
  }
  if (idx == 1) {
    stream << ",";
  }
  stream << ")";
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
  return LLVMIntType(32);
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

}  // namespace code
}  // namespace arrow
