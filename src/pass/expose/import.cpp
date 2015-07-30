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

namespace arrow {
namespace pass {

void Expose::visit_import(ast::Import& x) {
  // Build the absolute path of the file to import
  auto dir = fs::path(x.span.filename).parent_path();
  auto path = fs::absolute(x.source, dir);
  std::string pathname;

  try {
    // Try the exact filename
    pathname = fs::canonical(path).string();
  } catch (fs::filesystem_error) {
    try {
      // Append `.as`
      pathname = fs::canonical(path += ".as").string();
    } catch (fs::filesystem_error) {
      Log::get().error(
        "no module found for \"%s\"", x.source.c_str());

      return;
    }
  }

  // TODO: Check if this has been imported before
  // TODO: Make available in the current scope

  // Parse the AST from this file
  auto input_fs = new std::ifstream(pathname);
  std::shared_ptr<std::istream> input_stream(input_fs);
  arrow::Parser parser(input_stream, pathname);
  auto imported_module = parser.parse().as<ast::Module>();

  // Create (and emplace) the module item
  _ctx.modules[imported_module.get()] = new code::Module(
    imported_module, imported_module->name, nullptr, _scope);

  // Expose the now-imported module
  Expose(_ctx, _scope).run(*imported_module);

  // Emplace the imported module in scope of the current module
  auto mod_item = _ctx.modules[imported_module.get()];
  _scope->insert(new code::Import(&x, x.name, mod_item));
}

}  // namespace pass
}  // namespace arrow
