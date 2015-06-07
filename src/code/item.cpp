// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/item.hpp"

using arrow::code::Item;

Item::Item(std::shared_ptr<ast::Node> context) : context{context} {
}

Item::~Item() noexcept {
}
