// extern def puts(s: str);
extern def exit(status: int32);

def assert(condition: bool) {
  if condition {
    // Do nothing
    // TODO: Negation / Unless
  } else {
    exit(1);
  }
}

// def main() {
//   // assert(false);
//   let x = true;
//   if x {
//     puts("Sghs");
//   }

// return 23;


def main(argc: int32) -> int8 {
  assert(false);
  return 0;
}

// }
//
//
// // int8
// // int16
// // int32
// // int64
// // int128
// // uint8
// // uint16
// // uint32
// // uint64
// // uint128
// // float32
// // float64
// str
// bool
// char
//
// int
// float
// str
// bool
// char
