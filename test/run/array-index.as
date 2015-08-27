// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let mutable arr = [1, 2, 3];
  let val = arr[1];

  assert(val == 2);

  arr[1] = 59;

  assert(arr[1] == 59);
}
