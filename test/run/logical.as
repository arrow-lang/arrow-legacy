// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def test_bool() {
  assert(true == true);
  assert(false != true);
  assert(not true == false);

  assert(not (true < false));
  assert(not (true <= false));
  assert(true > false);
  assert(true >= false);

  assert(false < true);
  assert(false <= true);
  assert(not (false > true));
  assert(not (false >= true));

  assert(not (false > true) & not (true < false));
  assert((false > true) | (true > false));
  assert((false < true) & (true > false));
  assert((not false) & true);
  assert(true & (not false));

  assert((false > true) or true);
  assert(true or (false > true));
  assert(not true or true);
  assert(true or not true);
}

def test_int() {
  assert(12 == 12);
  assert(12 != 40);
  assert(not (12 == 40));

  assert(12 < 40);
  assert(40 > 12);
  assert(40 >= 40);
  assert(40 >= 34);
  assert(32 <= 32);
  assert(12 <= 32);

  assert((50 > 20) | (30 < 20));
  assert(not (40 == 30) & (30 > 20));
}

def main() -> int32 {
  test_bool();
  test_int();

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
