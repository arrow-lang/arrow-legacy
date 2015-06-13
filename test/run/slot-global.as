// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

let value = 30;
let flag: bool = false;

def main() {
  assert(not flag);
  assert(value == 30);
}
