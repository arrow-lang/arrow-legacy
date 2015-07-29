
// -----------------------------------------------------------------------------
// Case 2

let a2;

def methodA() {
  if false {
    methodA(); // error
  } else {
    a2 = 40;
  }
}

// -----------------------------------------------------------------------------
// Case 3

let a3;

def methodA() {
  a3 = 10;
  methodB(); // error
}

def methodB() {
  a3 = 50;
  methodA(); // error
}
