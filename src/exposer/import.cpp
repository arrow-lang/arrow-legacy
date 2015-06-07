// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/exposer.hpp"
#include "arrow/parser.hpp"
#include "arrow/tokenizer.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::resolve;
using arrow::Exposer;
namespace fs = boost::filesystem;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Exposer::visit_import(ast::Import& x) {
  // TODO: Directory import

  // Build the absolute path of the file to import
  // TODO: Get the relative path of THIS module file
  auto dir = fs::path(x.span.filename).parent_path();
  auto path = fs::absolute(x.path->text(), dir);
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
        "no module found for \"%s\"", x.path->text().c_str());

      return;
    }
  }

  // TODO: Check if this has been imported before
    // TODO: Make available in the current scope

  // Parse the AST from this file
  arrow::Tokenizer tokenizer{pathname};
  auto imported_module = std::dynamic_pointer_cast<ast::Module>(
    arrow::Parser(tokenizer).parse());

  // TODO: Push the imported module in scope of the current module

  // Remember that we imported this module (for next time)
  _g._imported_modules.emplace(pathname, imported_module);

  // Construct the exposer and run it over the module
  arrow::Exposer{_g, _scope}.run(*imported_module);
}
