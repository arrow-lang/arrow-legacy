from __future__ import print_function
from subprocess import Popen, PIPE
from os import path
import sys


WAF_TOOLS = ["compiler_cxx", "boost"]


def options(ctx):
    ctx.load(" ".join(WAF_TOOLS))


def configure(ctx):
    ctx.load(" ".join(WAF_TOOLS))

    ctx.check_boost(lib="system program_options")

    if ctx.env["CXX_NAME"] == "gcc":
        ctx.env.append_unique("CXXFLAGS", "-std=c++14")
        ctx.env.append_unique("CXXFLAGS", "-Wall")
        ctx.env.append_unique("CXXFLAGS", "-Wextra")


def build(ctx):
    ctx.program(source=ctx.path.ant_glob("src/**/*.cpp"),
                includes=["include"],
                target="arrow",
                use="BOOST")


def _get_expected_stdout(filename):
    fn = path.splitext(filename)[0] + ".stdout"
    if path.exists(fn):
        with open(fn, "rb") as stream:
            return stream.read().decode()

    return ""


_passed = 0
_failed = 0


def _test_print(file_, status):
    global _passed
    global _failed

    filename = path.relpath(file_.abspath())
    if status:
        _passed += 1
        print("{:<72}: \033[32m{}\033[0m".format(filename, 'PASS'))

    else:
        _failed += 1
        print("\033[31m{:<72}: {}\033[0m".format(filename, 'FAIL'))


def _print_sep(msg, sep="-", width=80):
    width -= len(msg) + 2
    left = right = width // 2
    if width % 2 != 0:
        right += 1
    print(sep * left, msg, sep * right)

def _print_report():
    print()

    message = []
    if _passed:
        message.append("{} passed".format(_passed))

    if _failed:
        message.append("{} failed".format(_failed))

    message = ', '.join(message)

    if not _failed:
        sys.stdout.write("\033[1;32m")
        _print_sep(message, sep='=')
        sys.stdout.write("\033[0m")
    else:
        sys.stdout.write("\033[1;31m")
        _print_sep(message, sep='=')
        sys.stdout.write("\033[0m")

def test(ctx):
    _print_sep("test session starts", "=")
    _print_sep("tokenize")

    binary = ctx.path.make_node("build/arrow").abspath()
    for file_ in ctx.path.ant_glob("test/tokenize/*.as"):
        # Run the test
        filename = file_.abspath()
        process = Popen([binary, filename], stdout=PIPE, stderr=PIPE)
        stdout, _ = process.communicate()

        # Check against the expected output
        expected = _get_expected_stdout(filename)
        test = expected == stdout.decode()

        # Print result
        _test_print(file_, test)

    _print_report()
