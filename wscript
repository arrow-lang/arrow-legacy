# -*- coding: utf-8 -*-
from itertools import chain
import ws.test


WAF_TOOLS = ["compiler_cxx", "boost"]


def options(ctx):
    ctx.load(" ".join(WAF_TOOLS))

    ctx.add_option(
        '--with-llvm-config',
        action='store',
        default='llvm-config'
    )


def configure(ctx):
    ctx.load(" ".join(WAF_TOOLS))

    ctx.check(lib='pthread', mandatory=True, uselib_store='PTHREAD')
    ctx.check(lib='dl', mandatory=True, uselib_store='DL')
    ctx.check(lib='tinfo', mandatory=True, uselib_store='TINFO')
    ctx.check(lib='z', mandatory=True, uselib_store='Z')

    ctx.check_boost(lib="system program_options filesystem")

    ctx.check_cfg(
        path=ctx.options.with_llvm_config, package='',
        args='--ldflags --cflags --libs',
        uselib_store='LLVM'
    )

    if ctx.env["CXX_NAME"] == "gcc":
        ctx.env.append_unique("CXXFLAGS", "-std=c++14")
        ctx.env.append_unique("CXXFLAGS", "-Ofast")
        ctx.env.append_unique("CXXFLAGS", "-Wall")
        ctx.env.append_unique("CXXFLAGS", "-Wextra")
        ctx.env.append_unique("CXXFLAGS", "-Wfatal-errors")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-final-types")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-final-methods")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-override")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-attribute=pure")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-attribute=const")
        # ctx.env.append_unique("CXXFLAGS", "-Wsuggest-attribute=noreturn")
        # ctx.env.append_unique("CXXFLAGS", "-Weffc++")
        ctx.env.append_unique("CXXFLAGS", "-Wpedantic")
        ctx.env.append_unique("CXXFLAGS", "-Woverloaded-virtual")


def build(ctx):
    ctx.program(source=ctx.path.ant_glob("src/**/*.cpp"),
                includes=["include", "lib"],
                target="arrow",
                use=["BOOST", "LLVM", "PTHREAD", "DL", "TINFO", "Z"])


def test(ctx):
    ws.test.run(ctx)

def lint(ctx):
    import cpplint

    include = ctx.path.ant_glob("include/**/*.hpp")
    source = ctx.path.ant_glob("src/**/*.cpp")

    filenames = [x.abspath() for x in chain(source, include)]

    cpplint.ParseArguments(
      ["--filter=-runtime/references"] + filenames)

    cpplint._cpplint_state.ResetErrorCounts()

    for filename in filenames:
        cpplint.ProcessFile(filename, 0)

    cpplint._cpplint_state.PrintErrorCounts()
