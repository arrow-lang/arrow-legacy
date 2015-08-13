// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

struct Box { value: int }

def make(a: int) -> Box {
  return Box(a);
}

def get(a: Box) -> int {
  return Box(a.value).value;
}

def main() {
  let a = Box(10);
  let mutable b = make(20);
  b.value = get(a);

  assert(b.value == 10);
}
