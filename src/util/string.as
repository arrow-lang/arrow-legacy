import string from "../../lib/libc/string";

export def eq(a: str, b: str) -> bool {
  return string.strcmp(a, b) == 0;
}
