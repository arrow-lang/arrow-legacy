// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let x = 40;
  let mutable y: type(x) = 0;
  y += 20;
  let mutable z: type(&x);
  z = &y;

  assert(*z == y);
  assert(y == 20);
  assert(x == 40);
}
