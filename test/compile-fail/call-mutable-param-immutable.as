def method(mutable value: int) {
  value = 20;
}

let x = 10;
method(x); // error
