// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  let u8: uint8 = 10;
  let u16: uint16 = 2000;
  let u32: uint32 = 300000;
  let u64: uint64 = 4000000000;
  let u128: uint128 = 50000000000000000000;

  assert(u8 + 5 == 15);
  assert(u16 + 1000 == 3000);
  assert(u32 + 150000 == 450000);
  assert(u64 + 2000000000 == 6000000000);
  assert(u128 + 25000000000000000000 == 75000000000000000000);

  assert(u8 - 5 == 5);
  assert(u16 - 1000 == 1000);
  assert(u32 - 150000 == 150000);
  assert(u64 - 2000000000 == 2000000000);
  assert(u128 - 25000000000000000000 == 25000000000000000000);

  assert(u8 * 2 == 20);
  assert(u16 * 3 == 6000);
  assert(u32 * 4 == 1200000);
  assert(u64 * 5 == 20000000000);
  assert(u128 * 6 == 300000000000000000000);

  // NOTE: This is floor division
  assert(u8 / 2 == 5);
  assert(u16 / 3 == 666);
  assert(u32 / 4 == 75000);
  assert(u64 / 5 == 800000000);
  assert(u128 / 6 == 8333333333333333333);

  assert(u8 % 2 == 0);
  assert(u16 % 3 == 2);
  assert(u32 % 4 == 0);
  assert(u64 % 5 == 0);
  assert(u128 % 6 == 2);
}
