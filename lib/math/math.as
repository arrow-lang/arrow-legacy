extern def printf(format: str, a: int);

def main() {
  printf("pow %d\n", pow(3,2));
  printf("factorial %d\n", factorial(6));
  printf("floor %d\n", floor(3.1));
  printf("abs %d\n", abs(-3));
  printf("ceil %d\n", ceil(3.2));
  printf("round %d\n", round(4.8));
  printf("fmod %d\n", fmod(2.0, 3.0) as int);
  0;
}

def fmod(x: float, y: float) -> float {
  // IEEE Standard 754 declares x REM y as
  // the mathematical operation x - (round(x/y)*y)
  // but python uses floor?
  return x - y * (round(x / y) as float);
}

def round(f: float) -> int {
  let offset = f - floor(f) as float;
  if offset > 0.5 { return ceil(f); }
  return floor(f);
}

def ceil(f: float) -> int {
  let v: float = f * 10.0;
  if v as int  % 10 != 0 { return (f as int) + 1; }
  return f as int;
}

def floor(f: float) -> int {
  return f as int - (f as int % 1);
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
