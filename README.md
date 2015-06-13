Arrow [![Stories in Ready](https://badge.waffle.io/arrowlang/arrow.svg?label=ready&title=Ready)](http://waffle.io/arrowlang/arrow)
=====
[![Build Status](https://travis-ci.org/arrowlang/arrow.svg?branch=master)](https://travis-ci.org/arrowlang/arrow)
[![Coverage Status](https://coveralls.io/repos/arrowlang/arrow/badge.svg?branch=master)](https://coveralls.io/r/arrowlang/arrow?branch=master)

## Prerequisites

### Ubuntu 14.10

```bash
$ sudo apt-get install build-essential clang llvm-dev libboost-all-dev libgmp-dev libzip-dev
```

## Build

```bash
$ ./waf configure
$ ./waf build -p
```

## Test

```bash
$ ./waf test
```
