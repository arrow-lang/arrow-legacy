// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let x: int = if true { 10; } else { 30; };
  assert(x == 10);

  let y: int = -if x <= 10 { 50; } else { 20; };
  assert(y == -50);
}
