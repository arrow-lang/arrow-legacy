// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let val: int128 = 0;
  let sizeof = ((0 as *type(val)) + 1) as *byte - (0 as *byte);

  assert(sizeof == 16);
}
