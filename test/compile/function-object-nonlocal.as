let a;

def methodA() {
  a = 10;
}

let fn = methodA; // ok
fn(); // ok

let b = a; // ok
