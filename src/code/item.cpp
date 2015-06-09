// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/item.hpp"
#include "arrow/llvm.hpp"

using arrow::code::Item;

Item::Item(ast::Node* context, code::Scope* scope) : context{context}, scope{scope} {
}

Item::~Item() noexcept {
}
