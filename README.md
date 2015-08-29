Arrow
=====
[![Build Status](https://travis-ci.org/arrowlang/arrow.svg?branch=master)](https://travis-ci.org/arrowlang/arrow)
[![Coverage Status](https://coveralls.io/repos/arrowlang/arrow/badge.svg?branch=master)](https://coveralls.io/r/arrowlang/arrow?branch=master)

**Arrow** is a fast (as or faster than C) general-purpose programming language. It does not employ a garbage collector and has minimal runtime overhead.

## Build

### Linux

#### Dependencies

Make sure you have installed the dependencies:
 - `g++` 4.9.x
 - `python`
 - `git`

```bash
$ sudo apt-get install llvm-dev libboost-all-dev libgmp-dev libzip-dev
```

#### Clone

```
$ git clone https://github.com/arrow-lang/arrow.git
$ cd arrow
```

#### Build

```bash
$ script/build
```

#### Install

```bash
$ sudo ./waf install
```

## License

**Arrow** is primarily distributed under the terms of the [MIT license](http://opensource.org/licenses/MIT).
