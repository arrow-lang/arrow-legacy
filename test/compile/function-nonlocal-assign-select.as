extern def exit(a: int);

let a;

def method() {
  if false {
    a = 34;
  } else if true {
    if false {
      a = 30;
    } else {
      a = 30;
    }
  } else {
    a = 40;
  }
}

method();

let b = a; // ok

exit(b);
