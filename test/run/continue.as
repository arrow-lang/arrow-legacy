// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let mutable counter = 10;
  let mutable foo = 321;
  while counter != 0 {
    counter = counter - 1;
    continue;
    foo = 123;
  }
  assert(foo == 321);
}
