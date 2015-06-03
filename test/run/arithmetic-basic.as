// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int32 {
  let a: int8 = 10;
  assert(a == 10);
  assert(a * (a - 1) == 90);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
