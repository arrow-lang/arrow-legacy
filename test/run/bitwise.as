// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int {
  let mutable a: int32 = 1;
  let mutable b: int32 = 2;
  a = a ^ b;
  b = b ^ a;
  a = a ^ b;
  assert(b == 1);
  assert(a == 2);
  assert(!0xf0 & 0xff == 0xf);
  assert(0xf0 | 0xf == 0xff);
  assert(0b1010_1010 | 0b0101_0101 == 0xff);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
