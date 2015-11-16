// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>
#include "arrow/code/type.hpp"
#include "arrow/match.hpp"

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

std::string TypeMember::name() const {
  std::stringstream stream;
  if (keyword.size() > 0) stream << keyword << ": ";
  stream << type->name();
  return stream.str();
}

std::string TypePointer::name() const {
  std::stringstream stream;
  stream << "*";
  if (is_mutable) stream << "mutable ";
  stream << pointee->name();
  return stream.str();
}

std::string TypeArray::name() const {
  std::stringstream stream;
  stream << element->name();
  stream << "[";
  stream << size;
  stream << "]";
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
  // Check if we are comparing against a byte
  if (typeid(other) == typeid(code::TypeByte)) {
    return bits == 8 && !is_signed;
  }

  // Check that we are comparing against a sized-integer
  if (!Type::equals(other)) return false;
  auto other_si = dynamic_cast<TypeSizedInteger&>(other);

  // Equal if 'bits' and 'signed' is equal
  return bits == other_si.bits && is_signed == other_si.is_signed;
}

bool TypeByte::equals(Type& other) const {
  // Check if we are comparing against a sized-integer
  if (typeid(other) == typeid(code::TypeSizedInteger)) {
    auto other_si = dynamic_cast<TypeSizedInteger&>(other);
    return other_si.bits == 8 && !other_si.is_signed;
  }

  // Check that we are comparing against a byte
  if (!Type::equals(other)) return false;

  return true;
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

bool TypePointer::equals(Type& other) const {
  // Check that we are comparing against a pointer
  if (!Type::equals(other)) return false;
  auto other_p = dynamic_cast<TypePointer&>(other);

  // Equal if the pointee's and mutability are eqiuvalent.
  return is_mutable == other_p.is_mutable &&
         pointee->equals(*other_p.pointee);
}

bool TypeArray::equals(Type& other) const {
  // Check that we are comparing against an array
  if (!Type::equals(other)) return false;
  auto other_a = dynamic_cast<TypeArray&>(other);

  // Equal if the pointee's and mutability are eqiuvalent.
  return size == other_a.size &&
         element->equals(*other_a.element);
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

bool TypeStructure::equals(Type& other) const {
  // Check that we are comparing against a struct
  if (!Type::equals(other)) return false;
  auto other_st = dynamic_cast<TypeStructure&>(other);

  // Only equal if we are us
  return other_st._handle == _handle;
}

bool TypeMember::equals(Type& other) const {
  // Check that we are comparing against a member
  if (!Type::equals(other)) return false;
  auto other_st = dynamic_cast<TypeMember&>(other);

  // Only equal if we are us
  return &other_st == this;
}

// Handle
// -----------------------------------------------------------------------------

LLVMTypeRef TypeBoolean::handle() {
  return LLVMInt1Type();
}

LLVMTypeRef TypeFloat::handle() {
  return LLVMDoubleType();
}

LLVMTypeRef TypeString::handle() {
  // TODO(_): Eventually we will use string /objects/ that contain the
  //          length.
  return LLVMPointerType(LLVMInt8Type(), 0);
}

LLVMTypeRef TypePointer::handle() {
  return LLVMPointerType(pointee->handle(), 0);
}

LLVMTypeRef TypeArray::handle() {
  return LLVMArrayType(element->handle(), size);
}

LLVMTypeRef TypeInteger::handle() {
  // TODO(mehcode): Arbitrary percision integer type
  return LLVMIntType(128);
}

LLVMTypeRef TypeIntegerLiteral::handle() {
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
      auto param_type_handle = param->type->handle();

      if (abi == Abi::Native && param->is_mutable) {
        param_type_handle = LLVMPointerType(param_type_handle, 0);
      }

      parameter_handles.push_back(param_type_handle);
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
  if (is_mutable) {
    return LLVMPointerType(type->handle(), 0);
  }

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

bool TypeMember::is_unknown() const {
  return type->is_unknown();
}

bool TypePointer::is_unknown() const {
  return pointee->is_unknown();
}

bool TypeArray::is_unknown() const {
  return element->is_unknown();
}

// Intersect
// -----------------------------------------------------------------------------

Ref<code::Type> TypeFloat::intersect(Ref<code::Type> other) const {
  // Float has the highest matching power
  Match(*other) {
    Case(TypeFloat& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeInteger& _) {
      XTL_UNUSED(_);
      return (new code::TypeFloat());
    }

    Case(TypeIntegerLiteral& _) {
      XTL_UNUSED(_);
      return (new code::TypeFloat());
    }

    Case(TypeSizedInteger& _) {
      XTL_UNUSED(_);
      return (new code::TypeFloat());
    }
  } EndMatch;

  return nullptr;
}

Ref<code::Type> TypeInteger::intersect(Ref<code::Type> other) const {
  Match(*other) {
    Case(TypeFloat& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeInteger& _) {
      XTL_UNUSED(_);
      return (new code::TypeInteger());
    }

    Case(TypeIntegerLiteral& _) {
      XTL_UNUSED(_);
      return (new code::TypeInteger());
    }

    Case(TypeSizedInteger& _) {
      XTL_UNUSED(_);
      return (new code::TypeInteger());
    }
  } EndMatch;

  return nullptr;
}

Ref<code::Type> TypeIntegerLiteral::intersect(Ref<code::Type> other) const {
  Match(*other) {
    Case(TypeFloat& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeInteger& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeFloat& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeIntegerLiteral& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeSizedInteger& _) {
      XTL_UNUSED(_);
      return other;
    }
  } EndMatch;

  return nullptr;
}

Ref<code::Type> TypeSizedInteger::intersect(Ref<code::Type> other) const {
  Match(*other) {
    Case(TypeFloat& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeInteger& _) {
      XTL_UNUSED(_);
      return other;
    }

    Case(TypeIntegerLiteral& _) {
      XTL_UNUSED(_);
      return (new code::TypeSizedInteger(bits, is_signed));
    }

    Case(TypeSizedInteger& x) {
      if (is_signed == x.is_signed) {
        return (bits > x.bits) ?
          (new code::TypeSizedInteger(bits, is_signed)) : other;
     }

      if (is_signed && bits > x.bits) {
        return (new code::TypeSizedInteger(bits, is_signed));
      }

      if (x.is_signed && x.bits > bits) {
        return other;
      }
    }
  } EndMatch;

  return nullptr;
}

Ref<code::Type> intersect_all(const std::vector<Ref<code::Type>>& types) {
  // If we have no types; return nil
  if (types.size() == 0) return nullptr;

  // If we have a single type; return that
  if (types.size() == 1) return types.at(0);

  // Reduce the list of types
  auto result = types.at(0);
  for (unsigned i = 1; i < types.size(); ++i) {
    result = result->intersect(types.at(i));
    if (!result) {
      break;
    }
  }

  return result;
}


// Util
// -----------------------------------------------------------------------------

Ref<code::TypeMember> TypeStructure::find_member(const std::string& name) {
  for (auto& mem : members) {
    if (mem->keyword == name) {
      return mem;
    }
  }

  return nullptr;
}

Ref<code::TypeParameter> TypeFunction::find_parameter(const std::string& name) {
  for (auto& param : parameters) {
    if (param->keyword == name) {
      return param;
    }
  }

  return nullptr;
}

}  // namespace code
}  // namespace arrow
