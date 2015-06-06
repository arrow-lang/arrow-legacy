// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def test_assign() {
  let mutable x: int;
  let y: int = x = 10;
  let mutable z: int;
  assert(x == 10);
  assert(y == 10);
  z = x = 11;
  assert(x == 11);
  assert(z == 11);
  z = x = 12;
  assert(x == 12);
  assert(z == 12);
}

def test_assign_op() {
  let mutable x: int = 0;
  let y: int = x += 10;
  let mutable z: int;
  assert(x == 10);
  assert(y == 10);
  z = x += 11;
  assert(x == 21);
  assert(z == 21);
  z = x += 12;
  assert(x == 33);
  assert(z == 33);
}

def main() {
  test_assign();
  test_assign_op();
}
