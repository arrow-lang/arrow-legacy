extern def exit(s: int);

let value: int;
def get() -> int { return value; }

value = 20; // ok
let value = get(); // ok

exit(value);
