// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def get() -> int {
  return 42;
}

def main() {
  assert(get() == 42);
}
