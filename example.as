let mutable a: int;

def do(_: int) { }

// use: `a`
def methodA() {
  if false {
    methodC();
  } else {
    methodB();
  }
}

methodB(); // error
let b = a;
// methodC and methodB fully analyzed now

// use: `a`
def methodC() {
  methodA();
  a = 30;
}

// use: `a`
def methodB() {
  methodC();
}
