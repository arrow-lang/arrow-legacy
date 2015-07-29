let a: int;

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

// use: `a`
def methodC() {
  methodA();
  do(a);
}

// use: `a`
def methodB() {
  methodC();
}
