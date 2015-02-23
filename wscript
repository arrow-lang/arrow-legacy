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
        ctx.env.append_unique("CXXFLAGS", "-Wall")
        ctx.env.append_unique("CXXFLAGS", "-Wextra")


def build(ctx):
    ctx.program(source=ctx.path.ant_glob("src/**/*.cpp"),
                includes=["include", "lib/utfcpp"],
                target="arrow",
                use="BOOST")


def test(ctx):
    ws.test.run(ctx)
