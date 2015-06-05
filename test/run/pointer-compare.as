// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int {
  let x = 10;
  let y = &x;
  let a = &x;
  let z = y;
  let w = &z;
  let e = &a;

  assert(y == z);
  assert(y == a);
  assert(e != w);

  return 0;
}
