// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def factorial(n: int64) -> int64 {
  // TODO: return n * factorial(n - 1) if n != 0 else 1;
  if n != 0 {
    return n * factorial(n - 1);
  } else {
    return 1;
  }
}

def main() -> int32 {
  assert(factorial( 0) ==       1);
  assert(factorial( 1) ==       1);
  assert(factorial( 2) ==       2);
  assert(factorial( 3) ==       6);
  assert(factorial( 4) ==      24);
  assert(factorial( 5) ==     120);
  assert(factorial( 6) ==     720);
  assert(factorial( 7) ==    5040);
  assert(factorial( 8) ==   40320);
  assert(factorial( 9) ==  362880);
  assert(factorial(10) == 3628800);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
