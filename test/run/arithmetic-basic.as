// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let a: int8 = 10;
  assert(a == 10);
  assert(a * (a - 1) == 90);
}
