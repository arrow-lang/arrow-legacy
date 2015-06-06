// Implementation of `usr/bin/env` in arrow
extern def puts(s: str);
def main(argc: int32, argv: *str, environ: *str) {
  let mutable index = 0;
  while *((environ + index) as **int8) != (0 as *int8) {
    puts(*(environ + index));
    index += 1;
  }
}
