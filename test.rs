// fn main() {
//     // let value: i8;
//     let value =342;
//     // let a;
//     // let box_ = (40,);
//     // ((a,),) = box_;
//     // // let x;
//     // // let condition = false;
//     // // if (condition) {
//     // //     x = 450;
//     // // }
//     // // x = 40;
// }

fn call<F>(f: F) where F: Fn() {
  f();
}

fn main() {
    let a = 20;
    let b = &mut a;

    // call(|| {
    //   a = 20;
    // });


    // let b;
    // (if false { a } else { b }) = 30;
}
