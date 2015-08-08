// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let mutable counter = 5;
  let mutable foo = 10;
  let mutable ase = 123;
  while counter != 0 {
    counter = counter - 1;
    while foo != 0 {
      foo = foo - 1;
      break;
      ase = 321;
    }
  }
  assert(ase == 123);
  assert(foo == 5);
}
