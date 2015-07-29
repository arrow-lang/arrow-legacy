let a;
let mutable b = 0;

def methodB() {
  b = 30;
}

def methodD() {
  methodA();
}

def methodC() {
  methodD();
  b = 20;
}

def methodA() {
  if false {
    // b (definite) [!]
    // a (not definite), b (definite)
    (if true { methodC; } else { methodB; })();
  } else {
    a = 40;
  }
}

// 1
// X "uses" Y
// methodA(a: not definite): methodC (not definite), methodB (not definite)
// methodC(b: definite): methodA (definite)
// methodB(b: definite): [nil]

// 2
// X "uses" Y
// methodA(a: not definite, b: not definite): methodC (not definite)
// methodC(b: definite): methodD (definite)
// methodD(): methodA (definite)
// methodB(b: definite): [nil]




// [ ] Move function assign/use check to function "use" (not "call")


let value;

def method() { // []
  value = 10;
  method2(); // error
}

def method2() {
  method3();
}

def method3() {
  method();
}


// ============================


let val = method2
val()
