// AnalyzeUsage
//  [ ] Function
//    [x] Basic: nested analyze
//    [ ] Non-local assignment/use
//  [ ] Loop
//  [ ] Import
//  [ ] Pointers

// Build
//  [x] Function
//  [ ] Return (Explicit)
//  [ ] Return (Implicit)
//
// // Case 1
// // ======
//
// let a;
//
// def method() {
//   a = 10;
// }
//
// method(); // ok
// method(); // error
//
// // Case 2
// // ======
//
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
//
// // possible assignment to A and B
// (if condition {
//   // ...
//   methodA;
// } else {
//   // ....
//   methodB;
// })(); // ok
//
// a = 20; // error
// b = 20; // error
// b; // error
// a; // error
//
// // Case 3
// // ======
//
// let a;
//
// def methodA() {
//   a = 10;
// }
//
// let fn = methodA; // ok
// fn(); // ok


// ---------------------------------------------------------------------------

// [ ] Record non-local use/assign on a per-function-TYPE basis
// [ ] On a CALL we use the function TYPE to determine if any use/assign
//     would be in conflict

// ---------------------------------------------------------------------------

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
