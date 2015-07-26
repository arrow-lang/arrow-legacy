
// -----------------------------------------------------------------------------
// Case 1

let a1;
let b1;

def methodA() {
  methodB(); // STOP and go analyze methodB
}

// check if we've been analyzed before analyzing
def methodB() {
  a1 = 20;
}

methodA(); // ok

b1 = a1; // ok

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






def test() {
}
