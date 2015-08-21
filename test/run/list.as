import libc from "./lib/libc";

def assert(condition: bool) {
  unless condition {
    libc.abort();
  }
}

struct List {
  _capacity: int,
  _size: int,
  _elements: *mutable int
}

def list_new() -> List {
  return List(
    0,
    0,
    0 as *mutable int
  );
}

def list_at(mutable self: List, index: int) -> int {
  return *(self._elements + index);
}

def list_reserve(mutable self: List, capacity: int) {
  // Ensure that we reserve space in chunks of 10 and update capacity.
  self._capacity = capacity + (10 - (capacity % 10));

  // Reallocate memory to the new requested capacity.
  let element_size = ((0 as *int) + 1) as *byte - (0 as *byte);
  self._elements = libc.realloc(
    self._elements as *mutable byte,
    (self._capacity * element_size) as int64) as *mutable int;
}

def list_push(mutable self: List, value: int) {
  // Reserve additional space (if needed)
  if self._capacity <= self._size {
    list_reserve(self, self._capacity + 1);
  }

  // Insert the value at the next available index
  *(self._elements + self._size) = value;

  // Increase our size
  self._size += 1;
}

def list_length(self: List) -> int {
  return self._size;
}

def list_pop(mutable self: List) -> int {
  return *(self._elements + (self._size -= 1));
}

def main() -> int {
  let mutable l = list_new();

  list_push(l, 10);
  list_push(l, 64);
  list_push(l, 1945);

  assert(list_at(l, 1) == 64);
  assert(list_length(l) == 3);
  assert(list_pop(l) == 1945);
  assert(list_length(l) == 2);

  return list_pop(l) - 64;
}
