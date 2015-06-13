// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  {
    {
      let mutable x = 20;
      let y = { x += 50; 10; };

      assert(x == 70);
      assert(y == 10);
    }
  }
}
