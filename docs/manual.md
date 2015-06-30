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
| terminal	         | `U+abcd`    |

## 2.2. String table productions

Some rules in the grammar are given in a simplified form: as a listing of a table of unquoted, printable whitespace-separated strings.

When such a string enclosed in double-quotes (`"`) occurs inside the grammar, it is an implicit reference to a single member of such a string table production. See tokens for more information.

# 3. Lexical structure

# 3.1. Input format

An input file is interpreted as a sequence of Unicode code points
encoded in UTF-8. Most grammar productions are defined in terms of
printable ASCII-range code points.

# 3.2. Comments

# 3.3. Statement termination (Semicolons)

The formal grammar use `;` (semicolon) as a terminator in most of its productions. Semicolons may be omitted when the statement termination coincides with the end of a line or an alternative terminator character (which would be: `]`, `}`, and `)`).

A semicolon may be manually used to terminate a statement before the end of a line to allow for multiple statements on a single line.

# 3.4. Whitespace

```
whitespace = U+0020 (* Space *)
           | U+0009 (* Horizontal tab *)
           | U+000b (* Vertical tab *)
           | U+000c (* Form feed *)
           ;
```

# 3.5. New lines

# 3.6. Identifiers

# 3.7. Keywords

```
and            as             break
continue       def            else
export         extern         false
from           global         if
import         let            loop
mutable        or             return
not            None           struct
true           type           unless
until          while
```
