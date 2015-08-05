// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_bool(ast::Boolean& x) {
  // Resolve the type of the literal boolean: `bool`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstInt(
    type->handle(), (x.value ? 1 : 0), false);

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_int(ast::Integer& x) {
  // Resolve the type of the literal integer
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstIntOfStringAndSize(
    type->handle(), x.text.c_str(), x.text.size(), 10);

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_float(ast::Float& x) {
  // Resolve the type of the literal float: `float`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstRealOfStringAndSize(
    type->handle(), x.text.c_str(), x.text.size());

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_none(ast::None&) {
  _stack.push_front(new code::ValueNone());
}

void Build::visit_str(ast::String& x) {
  // Resolve the type of the literal string: `str`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Unescape the string literal
  // NOTE: When we get string interpolation; this is where it'd go
  // TODO(mehcode): auto in_unicode_escape = false;
  std::vector<char> bytes;
  bool in_escape = false;
  auto in_byte_escape = false;
  for (unsigned i = 0; i < x.text.size(); ++i) {
    auto byte = x.text[i];
    if (in_escape) {
      // Check if we have an extension control character.
      switch (byte) {
        case 0x58:  // `X`
        case 0x78:  // `x`
          in_byte_escape = true;
          break;

        case 0x5c:  // `\`
          bytes.push_back('\\');
          break;

        case 0x27:  // `'`
          bytes.push_back('\'');
          break;

        case 0x22:  // `"`
          bytes.push_back('\"');
          break;

        case 0x61:  // `a`
          bytes.push_back('\a');
          break;

        case 0x62:  // `b`
          bytes.push_back('\b');
          break;

        case 0x66:  // `f`
          bytes.push_back('\f');
          break;

        case 0x6e:  // `n`
          bytes.push_back('\n');
          break;

        case 0x72:  // `r`
          bytes.push_back('\r');
          break;

        case 0x74:  // `t`
          bytes.push_back('\t');
          break;

        case 0x76:  // `v`
          bytes.push_back('\v');
          break;
      }

      // No longer in an escape sequence.
      in_escape = false;
    } else if (in_byte_escape) {
      std::stringstream convert;
      unsigned result = 0;
      convert << std::hex;
      convert << byte << x.text[i + 1];
      convert >> result;

      bytes.push_back(result);
      i += 1;

      // No longer in an byte escape sequence.
      in_byte_escape = false;
    } else {
      if (byte == 0x5c) {
        in_escape = true;
      } else {
        bytes.push_back(byte);
      }
    }
  }

  // Build the LLVM constant
  bytes.push_back('\0');
  auto handle = LLVMBuildGlobalStringPtr(_ctx.irb, bytes.data(), "");

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

}  // namespace pass
}  // namespace arrow
