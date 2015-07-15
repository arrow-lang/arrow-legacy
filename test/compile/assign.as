let mutable a = 30;
a = 40;

let mutable box = ((1,), a, true);
let x: int;
let y: int;
let z: bool;
((x,), y, z) = box;
