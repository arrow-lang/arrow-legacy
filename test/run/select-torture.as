// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def test_if() {
  let rs: bool = if true { true; } else { false; };
  assert(rs);
}

def test_else() {
  let rs: bool = if false { false; } else { true; };
  assert(rs);
}

def test_elseif1() {
  let rs: bool = if true { true; } else if true { false; } else { false; };
  assert(rs);
}

def test_elseif2() {
  let rs: bool = if false { false; } else if true { true; } else { false; };
  assert(rs);
}

def test_elseif3() {
  let rs: bool = if false { false; } else if false { false; } else { true; };
  assert(rs);
}

def test_inferrence() {
  let rs = if true { true; } else { false; };
  assert(rs);
}

def test_if_as_if_condition() {
  let rs1: bool = if if false { false; } else { true; } { true; } else { false; };
  assert(rs1);
  let rs2: bool = if if true { false; } else { true; } { false; } else { true; };
  assert(rs2);
}

def test_if_as_block_result() {
  let rs: bool = if true { if false { false; } else { true; }; } else { false; };
  assert(rs);
}

def main() -> int {
  test_if();
  test_else();
  test_elseif1();
  test_elseif2();
  test_elseif3();
  test_inferrence();
  test_if_as_if_condition();
  test_if_as_block_result();

  return 0;
}
