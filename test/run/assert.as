// TODO: Move into prelude
extern def exit(status: int32);
def assert(condition: bool) {
  if not condition {
    exit(1);
  }
}

def main() -> int32 {
  // Assert that the universe is still valid and consistent according to
  // our understand of its workings.
  assert(true);

  // TODO: Main should return 0 if we don't specify a return type
  return 0;
}
