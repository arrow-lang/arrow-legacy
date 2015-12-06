# 1. Introduction

# 2. Notation

The syntax notation for this manual uses [Extended Backus–Naur Form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form) (EBNF) as specified in ISO/IEC 14977 modified slightly to represent
terminals as UTF-32 (in hex notation).

## 2.1. Symbols

| Usage              | Notation    |
|--------------------|:-----------:|
| defintion          | `=`         |
| concatenation      | `,`         |
| termination        | `;`         |
| alternation        | `|`         |
| option             | `[ ... ]`   |
| repetition	       | `{ ... }`   |
| grouping  	       | `( ... )`   |
| comment            | `(* ... *)` |
| terminal	         | `U+ABCD`    |

## 2.2. String table productions

Some rules in the grammar are given in a simplified form: as a listing of a table of unquoted, printable whitespace-separated strings.

When such a string enclosed in double-quotes (`"`) occurs inside the grammar, it is an implicit reference to a single member of such a string table production. See tokens for more information.

# 3. Lexical structure

# 3.1. Input format

An input file is interpreted as a sequence of Unicode code points
encoded in UTF-8 (normalized before analysis using [NFKC](http://www.unicode.org/reports/tr15/)).
Most grammar productions are defined in terms of printable ASCII-range code points.

# 3.2. Comments

# 3.3. Statement termination (Semicolons)

The formal grammar use `;` (semicolon) as a terminator in most of its productions. Semicolons may be omitted when the statement termination coincides with a new line (`\n`) or an alternative terminator character (which would be: `]`, `}`, and `)`); and, the statement that is terminated would not be ill-formed.

A semicolon may be manually used to terminate a statement before the end of a line to allow for multiple statements on a single line.

# 3.4. Whitespace

```
whitespace = U+0020 (* Space *)
           | U+0009 (* Horizontal tab *)
           | U+000B (* Vertical tab *)
           | U+000C (* Form feed *)
           ;
```

# 3.5. New lines

```
eol = U+000A ;
```

# 3.6. Identifiers

Each character in an identifier shall designate a character whose encoding
in ISO/IEC 10646 falls into one of the ranges specified:

```
1. 00A8, 00AA, 00AD, 00AF, 00B2−00B5, 00B7−00BA, 00BC−00BE, 00C0−00D6,
   00D8−00F6, 00F8−00FF

2. 0100−167F, 1681−180D, 180F−1FFF

3. 200B−200D, 202A−202E, 203F−2040, 2054, 2060−206F

4. 2070−218F, 2460−24FF, 2776−2793, 2C00−2DFF, 2E80−2FFF

5. 3004−3007, 3021−302F, 3031−303F

6. 3040−D7FF

7. F900−FD3D, FD40−FDCF, FDF0−FE44, FE47−FFFD

8. 10000−1FFFD, 20000−2FFFD, 30000−3FFFD, 40000−4FFFD, 50000−5FFFD,
   60000−6FFFD, 70000−7FFFD, 80000−8FFFD, 90000−9FFFD, A0000−AFFFD,
   B0000−BFFFD, C0000−CFFFD, D0000−DFFFD, E0000−EFFFD
```

The initial character shall not be a universal character
name designating a character whose encoding falls into one of the
ranges specified:

```
0300−036F, 1DC0−1DFF, 20D0−20FF, FE20−FE2F
```

# 3.7. Keywords

```
```

# 3.8. Punctuators

```
```

A punctuator is a symbol that has independent syntactic and semantic
significance.
