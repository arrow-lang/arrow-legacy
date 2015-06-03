// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int32 {
  let i8: int8 = 10;
  let i16: int16 = 2000;
  let i32: int32 = 300000;
  let i64: int64 = 4000000000;
  let i128: int128 = 50000000000000000000;

  assert(i8 + 5 == 15);
  assert(i16 + 1000 == 3000);
  assert(i32 + 150000 == 450000);
  assert(i64 + 2000000000 == 6000000000);
  assert(i128 + 25000000000000000000 == 75000000000000000000);

  assert(i8 - 5 == 5);
  assert(i16 - 1000 == 1000);
  assert(i32 - 150000 == 150000);
  assert(i64 - 2000000000 == 2000000000);
  assert(i128 - 25000000000000000000 == 25000000000000000000);

  assert(i8 * 2 == 20);
  assert(i16 * 3 == 6000);
  assert(i32 * 4 == 1200000);
  assert(i64 * 5 == 20000000000);
  assert(i128 * 6 == 300000000000000000000);

  // NOTE: This is floor division
  assert(i8 / 2 == 5);
  assert(i16 / 3 == 666);
  assert(i32 / 4 == 75000);
  assert(i64 / 5 == 800000000);
  assert(i128 / 6 == 8333333333333333333);

  assert(i8 % 2 == 0);
  assert(i16 % 3 == 2);
  assert(i32 % 4 == 0);
  assert(i64 % 5 == 0);
  assert(i128 % 6 == 2);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
