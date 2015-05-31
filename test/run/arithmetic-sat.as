// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def sat_add(x: uint32) -> uint32 {
  let ret: uint32 = x + 1;
  if ret < x { return x; };
  return ret;
}

def main() -> uint32 {
  let neg: uint32 = -1;
  assert(sat_add(neg) == neg);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
