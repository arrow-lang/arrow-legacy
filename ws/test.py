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
        handler = globals()["handle_%s" % name.replace("-", "_")]
        test = handler(binary_path, file_.abspath())

        # Print result
        print_test(file_, test)


def handle_(binary_path, filename, *args):
    filename = path.relpath(filename)
    process = Popen(
        [binary_path] + list(args) + [filename], stdout=PIPE, stderr=PIPE,
        cwd=path.join(path.dirname(__file__), ".."),
    )

    stdout, _ = process.communicate()

    expected = get_expected(filename, "stdout")
    test = expected == stdout.decode('utf-8') and process.returncode == 0

    return test


def handle_fail_(binary_path, filename, *args):
    filename = path.relpath(filename)
    process = Popen(
        [binary_path] + list(args) + [filename], stdout=PIPE, stderr=PIPE,
        cwd=path.join(path.dirname(__file__), ".."),
    )

    _, stderr = process.communicate()

    # TODO: expected = get_expected(filename, "stderr")
    test = process.returncode != 0

    return test


def handle_read(binary_path, filename):
    return handle_(binary_path, filename, "--read")


def handle_tokenize(binary_path, filename):
    return handle_(binary_path, filename, "--tokenize")


def handle_tokenize_fail(binary_path, filename):
    return handle_fail_(binary_path, filename, "--tokenize")


def handle_parse(binary_path, filename):
    return handle_(binary_path, filename, "--parse")


def handle_parse_fail(binary_path, filename):
    return handle_fail_(binary_path, filename, "--parse")


def handle_run(binary_path, filename):
    filename = path.relpath(filename)
    p = Popen(
        [binary_path, filename], stdout=PIPE, stderr=PIPE,
        cwd=path.join(path.dirname(__file__), ".."),
    )
    interpreter = Popen(["lli"], stdin=p.stdout, stdout=PIPE, stderr=PIPE)

    stdout, _ = interpreter.communicate()

    expected = get_expected(filename, "stdout")
    test = expected == stdout.decode('utf-8') and interpreter.returncode == 0

    return test


def handle_run_fail(binary_path, filename):
    filename = path.relpath(filename)
    p = Popen(
        [binary_path, filename], stdout=PIPE, stderr=PIPE,
        cwd=path.join(path.dirname(__file__), ".."),
    )
    interpreter = Popen(["lli"], stdin=p.stdout, stdout=PIPE, stderr=PIPE)

    stdout, _ = interpreter.communicate()

    expected = get_expected(filename, "stdout")
    test = expected == stdout.decode('utf-8') and interpreter.returncode != 0

    return test


def run(ctx):
    print_sep("test session starts", "=", end="")

    binary_path = ctx.path.make_node("build/arrow").abspath()

    run_("read", ctx, binary_path)
    run_("tokenize", ctx, binary_path)
    run_("tokenize-fail", ctx, binary_path)
    run_("parse", ctx, binary_path)
    run_("parse-fail", ctx, binary_path)
    # run_("run", ctx, binary_path)
    # run_("run-fail", ctx, binary_path)

    print_report()

    return _failed == 0
