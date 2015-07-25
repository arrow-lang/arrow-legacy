// Case 2
// ======

// let a;
// let b;
//
// def methodA() {
//   a = 10;
// }
//
// def methodB() {
//   b = 10;
// }

// possible assignment to A and B
// (if false {
//   // ...
//   methodA;
// } else {
//   // ....
//   methodB;
// })(); // ok

// a = 20; // error
// b = 20; // error
// b; // error
// a; // error

// // Case 1 (error)
// // =====================
//
// // b.as
// export let value;
// // [...]
// // error: exported variable was not assigned
//
// // a.as
// import b from "./b";
// b.x = 10;  // error: assignment to immutable variable
//
// // Case 2 (ok)
// // =====================
//
// // math.as
// export let PI = (22.0 / 7.0);
//
// // a.as
// import math from "./math";
// let radius = 5.12;
// let circumference = math.PI * radius * 2;
//
// // Case 3 (??)
// // =====================
//
// // b.as
// import a from "./a";
// export let one = a.x;
// export let two = 50;
//
// // a.as
// import b from "./b";
// export let x = b.two;
//
//
// // Case 4 (error)
// // =====================
//
// // a.as
// export let val;
// export def method() {
//   val = a.x;
// }
//
// // [...]
// // error: exported variable was not assigned
//
//
// // Case 5 (ok)
// // =====================
//
// // a.as
// export let val;
// export def method() {
//   val = a.x;
// }
//
// method();
//
//
// // Case 6 (ok)
// // =====================
//
// // a.as
// import b from "./b";
// export def one() {
//   b.two();
// }
//
// // b.as
// import a from "./a";
// export def two() {
//   a.one();
// }
