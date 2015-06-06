// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() {
  if false {
    assert(false);
  } else if false {
    assert(false);
  } else if false { assert(false); } else { assert(true); };
  if false { assert(false); } else if true { assert(true); };
  if false {
    assert(false);
  } else if true {
    if true {
      assert(true);
    } else { if false { assert(false); } else { assert(false); }; };
  };
  if false {
    assert(false);
  } else { if false { assert(false); } else { assert(true); }; };
}
