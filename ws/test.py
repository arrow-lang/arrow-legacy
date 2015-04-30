# -*- coding: utf-8 -*-
from __future__ import print_function
from subprocess import Popen, PIPE
from os import path
import sys

_passed = 0
_failed = 0


def get_expected(filename, item):
    fn = path.splitext(filename)[0] + "." + item
    if path.exists(fn):
        with open(fn, "rb") as stream:
            return stream.read().decode('utf-8')

    return ""


def print_test(file_, status):
    global _passed
    global _failed

    filename = path.relpath(file_.abspath())
    if status:
        _passed += 1
        print("{:<72}: \033[32m{}\033[0m".format(filename, 'PASS'))

    else:
        _failed += 1
        print("\033[31m{:<72}: {}\033[0m".format(filename, 'FAIL'))


def print_sep(msg, sep="-", width=80, end="\n"):
    width -= len(msg) + 2
    left = right = width // 2

    if width % 2 != 0:
        right += 1

    print(sep * left, msg, sep * right, end=end)


def print_report():
    print()

    message = []
    if _passed:
        message.append("{} passed".format(_passed))

    if _failed:
        message.append("{} failed".format(_failed))

    message = ', '.join(message)

    if not _failed:
        sys.stdout.write("\033[1;32m")
        print_sep(message, sep='=')
        sys.stdout.write("\033[0m")
    else:
        sys.stdout.write("\033[1;31m")
        print_sep(message, sep='=')
        sys.stdout.write("\033[0m")


def run_(name, ctx, binary_path):
    print()
    print_sep(name)

    for file_ in ctx.path.ant_glob("test/%s/*.as" % name):
        # Execute the handler
        handler = globals()["handle_%s" % name]
        test = handler(binary_path, file_.abspath())

        # Print result
        print_test(file_, test)


def handle_(binary_path, filename, *args):
    filename = path.relpath(filename)
    process = Popen(
        [binary_path, filename] + list(args), stdout=PIPE, stderr=PIPE,
        cwd=path.join(path.dirname(__file__), ".."),
        # shell=True
    )

    stdout, _ = process.communicate()

    expected = get_expected(filename, "stdout")
    test = expected == stdout.decode('utf-8')

    return test


def handle_tokenize(binary_path, filename):
    return handle_(binary_path, filename, "--tokenize")


def handle_parse(binary_path, filename):
    return handle_(binary_path, filename, "--parse")


def run(ctx):
    print_sep("test session starts", "=", end="")

    binary_path = ctx.path.make_node("build/arrow").abspath()

    run_("tokenize", ctx, binary_path)
    run_("parse", ctx, binary_path)

    print_report()

    return _failed == 0
