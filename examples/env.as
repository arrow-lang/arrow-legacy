// Implementation of `usr/bin/env` in arrow
let nil = 0 as *int8;
extern def puts(s: str);
def main(argc: int32, argv: *str, environ: *str) {
  let mutable index = 0;
  while *((environ + index) as **int8) != nil {
    puts(*(environ + index));
    index += 1;
  }
}
