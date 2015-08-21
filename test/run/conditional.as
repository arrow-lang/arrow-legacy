// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int {
  assert((20 if true else 0) == 20);
  return (20 if false else 0);
}
