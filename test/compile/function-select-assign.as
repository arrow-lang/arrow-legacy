extern def exit(a: int);

def method() {
  let a;

  if false {
    a = 34;
  } else if true {
    if false {
      a = 30;
    } else {
      a = 32;
    }
  } else {
    a = 40;
  }

  let b = a; // ok

  exit(b);
}

method();
