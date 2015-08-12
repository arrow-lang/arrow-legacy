extern def printf(format: str, a: int);

def main() {
  // Bug?
  let floorVal = -2.7;
  let ceilVal = -2.2;

  printf("pow %d\n", pow(3,2));
  printf("factorial %d\n", factorial(6));
  printf("floor %d\n", floor(floorVal));
  printf("abs %d\n", abs(-3));
  printf("ceil %d\n", ceil(ceilVal));
  printf("round %d\n", round(4.8));
  printf("fmod %d\n", fmod(2.0, 3.0) as int);
  0;
}

def fmod(x: float, y: float) -> float {
  // IEEE Standard 754 declares x REM y as
  // the mathematical operation x - (round(x/y)*y)
  // but python uses floor?
  return x - y * (floor(x / y) as float);
}

def round(f: float) -> int {
  let offset = f - floor(f) as float;
  if offset > 0.5 { return ceil(f); }
  return floor(f);
}

def ceil(f: float) -> int {
  let v = f as int;
  if f < 0.0 { return v; }
  return v + 1;
}

def floor(f: float) -> int {
  let v = f as int;
  if f < 0.0 { return v - 1; }
  return v;
}

def abs(i: int) -> int {
  if i > 0 { return i; }
  return -i;
}

def factorial(i: int) -> int {
  if i != 1 { return i * factorial(i - 1); }
  return i;
}

// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
def pow(i: int, e: int) -> int {
  // Short circut
  if (e == 0) { return 1; }
  else if e % 2 == 1 { return i * pow(i, e - 1); }
  let p = pow(i, e / 2);

  return p * p;
}
