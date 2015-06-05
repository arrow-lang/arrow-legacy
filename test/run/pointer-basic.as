// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int {
  let a = 10;
  let b = &a;
  let c = *b;
  assert(c == a);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
