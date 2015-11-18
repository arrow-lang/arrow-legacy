# -*- coding: utf-8 -*-
import sys
import waflib.Scripting
import os
from os import path
import shutil
from subprocess import Popen, PIPE, check_output
from glob import glob


top = '.'
out = 'build'


def options(ctx):
    # Add an option to specify an existing arrow compiler for the boostrap
    # process.
    ctx.add_option("--with-arrow", action='store', dest='arrow')

    # Add a --with-llvm-config option to specify what binary to use here
    # instead of the standard `llvm-config`.
    ctx.add_option(
        '--with-llvm-config', action='store', default='llvm-config',
        dest='llvm_config')


def llvm_config(ctx, *args):
    command = [ctx.env.LLVM_CONFIG[0]]
    command.extend(args)
    return check_output(command).decode("utf-8").strip()


def configure(ctx):
    # Load preconfigured tools.
    ctx.load('c_config')

    # Report the compiler to be used.
    # ctx.msg("Checking for 'arrow' (Arrow compiler)")
    ctx.find_program('arrow', var='ARROW')

    # Check for the llvm compiler.
    ctx.find_program('llc', var='LLC')
    ctx.find_program('lli', var='LLI')
    ctx.find_program('opt', var='OPT')

    # Check for gcc.
    # NOTE: We only depend on this for the linking phase.
    ctx.find_program('gcc', var='GCC')
    ctx.find_program('g++', var='GXX')

    # Check for the `llvm-config`.
    ctx.find_program('llvm-config', var="LLVM_CONFIG")

    # Use `llvm-config` to discover configuration
    ctx.env.LLVM_LDFLAGS = llvm_config(ctx, "--ldflags")
    ctx.env.LLVM_LIBS = llvm_config(ctx, "--libs", "all")

    # Get the version of the current package.
    ver = check_output(
        ["git", "describe", "--always", "--tag"]).strip().decode()
    ver_parts = ver.split("-")
    if len(ver_parts) == 1:
        ctx.env.VERSION = ver
    else:
        ctx.env.VERSION = '+'.join([ver_parts[0],
                                    ver_parts[1] + "." + ver_parts[2]])


def _link(ctx, source, target, name):
    libs = ctx.env.LLVM_LIBS
    ldflags = ctx.env.LLVM_LDFLAGS
    rule = "${GXX} -o${TGT} ${SRC} %s %s" % (libs, ldflags)
    if sys.version_info[0] == 2:
        rule = rule.encode()
    ctx(rule=rule,
        source=source,
        target=target,
        name=name)


def build(ctx):
    # Write a configuration file that contains the version.
    cfg = path.join(ctx.bldnode.abspath(), "cfg/_version.as")
    try:
        os.makedirs(path.dirname(cfg))
    except:
        pass
    with open(cfg, "w") as stream:
        stream.write("let VERSION = '%s';\n" % ctx.env.VERSION)

    # Take the stage-1 compiler (the one that we have through
    # reasons unknown to us). Use this to compile the stage-2 compiler.

    # Compile the compiler to the llvm IL.
    ctx(rule="arrow --compile ${SRC} | ${OPT} -O3 -S -o=${TGT}",
        source="src/index.as",
        target="arrow.ll",
        after="stage0")

    # Compile the compiler from llvm IL into native object code.
    ctx(rule="${LLC} -filetype=obj -o=${TGT} ${SRC}",
        source="arrow.ll",
        target="arrow.o")

    # Link the compiler into a final executable.
    _link(ctx, "arrow.o", "arrow", "")
