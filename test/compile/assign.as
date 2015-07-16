let mutable a = 30;
a = 40;

let mutable box = ((1,), a, true);
let x;
let y;
let z;
((x,), y, z) = box;
