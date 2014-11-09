

WAF_TOOLS = ["compiler_cxx", "boost"]


def options(ctx):
    ctx.load(" ".join(WAF_TOOLS))


def configure(ctx):
    ctx.load(" ".join(WAF_TOOLS))


def build(ctx):
    ctx.program(source=ctx.path.ant_glob("src/**/*.cpp"),
                target="arrow")
