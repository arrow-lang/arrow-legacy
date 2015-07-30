// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>
#include <fstream>

#include "arrow/match.hpp"
#include "arrow/parser.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"

namespace fs = boost::filesystem;

static bool ends_with(const std::string& value, const std::string& ending) {
  if (ending.size() > value.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

namespace arrow {
namespace pass {

void Expose::visit_import(ast::Import& x) {
  // Build the absolute path of the file to import
  auto dir = fs::path(x.span.filename).parent_path();
  auto path = fs::absolute(x.source, dir);
  std::string pathname;

  // If 'source' did not end in '.as'; append '.as'
  if (!ends_with(x.source, ".as")) {
    path += ".as";
  }

  try {
    // Try the exact filename
    pathname = fs::canonical(path).string();
  } catch (fs::filesystem_error) {
    Log::get().error(
      "no module found for \"%s\"", x.source.c_str());

    return;
  }

  // Check if this has been imported before
  Ref<code::Module> mod_item = nullptr;
  auto ref = _ctx.modules_by_pathname.find(pathname);
  if (ref != _ctx.modules_by_pathname.end()) {
    mod_item = ref->second;
  } else {
    // Parse the AST from this file
    auto input_fs = new std::ifstream(pathname);
    std::shared_ptr<std::istream> input_stream(input_fs);
    arrow::Parser parser(input_stream, pathname);
    auto imp = parser.parse().as<ast::Module>();

    // Create (and emplace) the module item
    mod_item = new code::Module(imp, imp->name, nullptr, _scope);
    _ctx.modules_by_pathname[pathname] = mod_item;
    _ctx.modules[imp.get()] = mod_item;

    // Expose the now-imported module
    Expose(_ctx, _scope).run(*imp);
  }

  // Emplace the imported module in scope of the current module
  _scope->insert(new code::Import(&x, x.name, mod_item));
}

}  // namespace pass
}  // namespace arrow
