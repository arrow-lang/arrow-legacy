let mutable a: int;
let b: int;

def methodA() {
  methodB(); // STOP and go analyze methodB
}

// check if we've been analyzed before analyzing
def methodB() {
  a = 20;
}

methodA(); // ok

b = a; // ok
