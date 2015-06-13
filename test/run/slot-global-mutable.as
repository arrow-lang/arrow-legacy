// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

let mutable value = 30;

def main() {
  assert(value == 332);
}

value += 302;
