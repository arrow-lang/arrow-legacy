// Simple 2-field class
class Point {
  y: int;
  x: int;
}

// Class with default values for its fields
class Point {
  x: int = 0;
  y: int = 0;
}

Point(); // ok
Point(y=20); // ok
Point(x=20, y=50); // ok

// Simple class with a member function and an attached function
class Shape {
  name() -> str { return "Rectangle"; }
  is_square(self) -> bool { return false; }
}

Shape.name(); //= "Rectangle"
Shape().name(); // error
Shape().is_square(); //= false
Shape.is_square(); // error

// Class with both fields and functions
class Both {
  a: int;
  b() -> int { return a; }
}

// Generic
class Box<T> { value: T; }
class Point<X, Y> { x: X; y: Y; }

// Generic with restriction to type parameter (`T` must be able to satisfy
// the `Show` interface)
class Box<T: Show> { value: T; }

// The following is not equivalent to a generic type restriction.
// This will actually store a boxed value for the Show interface.
class Box { value: Show; }
