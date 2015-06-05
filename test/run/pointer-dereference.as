// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int {
  let mutable a = 230;
  let b = &mutable a;
  *b = 30;
  assert(a == 30);
  assert(*b == 30);
  ((*b)) = 610;
  assert(a == 610);
  assert(*b == 610);

  return 0;
}
