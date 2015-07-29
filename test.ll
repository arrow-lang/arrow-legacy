; ModuleID = 'index'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@a = internal constant i8 (i8)* @exit

define internal void @"index.@init"() {
  ret void
}

declare signext i8 @exit(i8 signext)

define i32 @main(i32, i8**, i8**) {
  call void @"index.@init"()
  ret i32 0
}
