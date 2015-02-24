# -*- coding: utf-8 -*-
import ws.test


WAF_TOOLS = ["compiler_cxx", "boost"]


def options(ctx):
    ctx.load(" ".join(WAF_TOOLS))


def configure(ctx):
    ctx.load(" ".join(WAF_TOOLS))

    ctx.check_boost(lib="system program_options filesystem")

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
        ctx.env.append_unique("CXXFLAGS", "-Weffc++")
        ctx.env.append_unique("CXXFLAGS", "-Wpedantic")
        ctx.env.append_unique("CXXFLAGS", "-Woverloaded-virtual")


def build(ctx):
    ctx.program(source=ctx.path.ant_glob("src/**/*.cpp"),
                includes=["include", "lib"],
                target="arrow",
                use="BOOST")


def test(ctx):
    ws.test.run(ctx)
