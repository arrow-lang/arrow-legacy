// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let mutable counter = 10;
  loop {
    counter -= 1;
    break;
  }
  assert(counter == 9);
}
