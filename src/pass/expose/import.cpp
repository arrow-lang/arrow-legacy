// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>
#include <fstream>

#include "arrow/util.hpp"
#include "arrow/parser.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"

namespace fs = boost::filesystem;

// NOTE: Taken from: http://stackoverflow.com/a/29221546
static fs::path relativeTo(fs::path from, fs::path to)
{
   // Start at the root path and while they are the same then do nothing then
   // when they first diverge take the remainder of the two path and replace
   // the entire from path with ".."
   // segments.
   fs::path::const_iterator fromIter = from.begin();
   fs::path::const_iterator toIter = to.begin();

   // Loop through both
   while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
   {
      ++toIter;
      ++fromIter;
   }

   fs::path finalPath;
   while (fromIter != from.end())
   {
      finalPath /= "..";
      ++fromIter;
   }

   while (toIter != to.end())
   {
      finalPath /= *toIter;
      ++toIter;
   }

   return finalPath;
}

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
      x.span, "no module found for \"%s\"", x.source.c_str());

    return;
  }

  // Check if this has been imported before (by the whole program)
  auto current_mod = util::current_module(_scope);
  Ref<code::Module> mod_item = nullptr;
  auto ref = _ctx.modules_by_pathname.find(pathname);
  if (ref != _ctx.modules_by_pathname.end()) {
    mod_item = ref->second;

    // Check if this is this module
    if (current_mod == mod_item.get()) {
      // It is an error to import yourself
      Log::get().error(x.span, "module cannot import itself");
      return;
    }

    // Check if this has been imported before (by this module)
    for (auto& imp : current_mod->imports) {
      if (imp->module.get() == mod_item.get()) {
        // It is an error to double import
        Log::get().error(
          x.span, "duplicate import for \"%s\"", x.source.c_str());

        return;
      }
    }
  } else {
    // Relativize the filename from the CWD
    boost::filesystem::path full_path(boost::filesystem::current_path());
    auto relpath = relativeTo(full_path, pathname);

    // Parse the AST from this file
    auto input_fs = new std::ifstream(pathname);
    std::shared_ptr<std::istream> input_stream(input_fs);
    arrow::Parser parser(input_stream, relpath.string());
    auto imp = parser.parse().as<ast::Module>();

    // Create (and emplace) the module item
    mod_item = new code::Module(imp, imp->name, nullptr, _scope);
    _ctx.modules_by_pathname[pathname] = mod_item;
    _ctx.modules_by_context[imp.get()] = mod_item;
    _ctx.modules.push_back(mod_item);

    // Expose the now-imported module
    Expose(_ctx, _scope).run(*imp);
  }

  // Emplace the imported module in scope of the current module
  Ref<code::Import> imported_item = new code::Import(&x, x.name, mod_item);
  _scope->insert(imported_item);
  current_mod->imports.insert(imported_item.get());
}

}  // namespace pass
}  // namespace arrow
