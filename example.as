def main() {
  sub();
}

def sub() {
}

// build([module#example]) ->
//  build([function#main]) ->
//   build([function#sub]) ->
//    build([function#main]) -> short-circuit


// build => responsible for two-fold;



// 3 + 2
// build([ast::Add]) => LLVMValue

// build([ast::Call])
//  build([ast::Call->expression])

resolve([ast::Call->expression])

// For each item ..
let mut items = [false];
for item in items {
  // .. negate and push back in list
  items.push(not item);

  // Break out if we have more than a million items
  if items.len() > 1e6 { break; }
}
