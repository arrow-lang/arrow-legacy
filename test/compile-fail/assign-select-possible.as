let a;

if false {
  a = 34;
} else if true {
  if false {
    a = 30;
  }
} else {
  a = 50;
}

let b = a; // error (branch 2 isn't definite)
