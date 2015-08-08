// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let a = 10;
  let b = &a;
  let c = *b;
  assert(c == a);
}
