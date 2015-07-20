// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/scope.hpp"

namespace arrow {
namespace code {

// Scope (tree of blocks)
// -----------------------------------------------------------------------------

// Scope::~Scope() noexcept {
// }
//
// void Scope::emplace(Ref<code::Item> item) {
//   // Has this item already been added ..
//   int index = -1;
//   if (item->context) {
//     auto ref = _items_by_ctx.find(item->context);
//     if (ref != _items_by_ctx.end()) {
//       // Yes; get the index and just re-activate in the name map
//       index = ref->second;
//     }
//   }
//
//   if (index == -1) {
//     // No; push and map into both
//     _items.push_back(item);
//     index = (_items.size() - 1);
//     _items_by_name[item->name] = index;
//     _items_by_ctx[item->context] = index;
//   } else {
//     _items_by_name[item->name] = index;
//   }
// }
//
// bool Scope::exists(ast::Node* context, bool traverse) {
//   return get(context, traverse, false) != nullptr;
// }
//
// bool Scope::exists(const std::string& name, bool traverse) {
//   return get(name, traverse) != nullptr;
// }
//
// auto Scope::get(ast::Node* context, bool traverse, bool unshadow)
//     -> Ref<code::Item> {
//   auto ref = _items_by_ctx.find(context);
//   if (ref == _items_by_ctx.end()) {
//     if (_parent && traverse) return _parent->get(context, traverse);
//     return nullptr;
//   }
//
//   // Emplace the name
//   // NOTE: Get by context is intended for visitors to explicitly
//   //       request an item (and at the same time pushing that item
//   //       to be unshadowed)
//   auto item = _items.at(ref->second);
//   if (unshadow) {
//     _items_by_name[item->name] = ref->second;
//   }
//
//   return item;
// }
//
// auto Scope::get(const std::string& name, bool traverse)
//     -> Ref<code::Item> {
//   auto ref = _items_by_name.find(name);
//   if (ref == _items_by_name.end()) {
//     if (_parent && traverse) return _parent->get(name, traverse);
//     return nullptr;
//   }
//
//   return _items.at(ref->second);
// }

// Block
// -----------------------------------------------------------------------------

}  // namespace code
}  // namespace arrow
