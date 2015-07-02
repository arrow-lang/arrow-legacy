let call(param: int) -> None {
}

let call(param: int) {
}

let list_map(x: int) -> int {
  return x * y;
}

let dict_map(value: int, key: str) -> int {
  return +value;
}

let default_arg(radix: int = 40) -> int {
  return radix;
}

let main() {
}

let ようこそ() {
}

let main() -> int {
  return 10;
}

let ようこそ() -> int {
  return 60;
}

// Generic
let add(x, y) -> type(x + y) { return x + y; }
let add(x, y) { return x + y; }

// Generic with restriction
// Anything that satisfies the `a` interface can be passed in to this function
let print(a: Show) -> { std.print(a); }
