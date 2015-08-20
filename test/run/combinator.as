// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let mutable counter = 0;

  if false or { counter += 1; true; } {
    assert(true);
  }

  if true or { counter += 1; false; } {
    assert(true);
  }

  if false and { counter += 1; true; } {
    assert(false);
  }

  if true and { counter += 1; true; } {
    assert(true);
  }

  assert(counter == 2);
}
