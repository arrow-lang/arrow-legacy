
let mutable a: uint8 = 10;
let mutable b: byte = 10;

b = a;
a = b;

let mutable ptr_a = &mutable a;
let mutable ptr_b = &mutable b;

ptr_a = ptr_b;
ptr_b = ptr_a;

let mutable arr_a = [a];
let mutable arr_b = [b];

arr_a = arr_b;
arr_b = arr_a;
