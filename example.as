let a;

if false {
  if true {
    a = 30;
  } else {
    a = 20;
  }
} else {
  // nothing
}

def method() {
  a = 30;
}

let b = a; // error: possible use before init


// 1 - when crossing named-scope boundries; an immutable variable (
//     even without assignments) is considered constant
// # > assignment to non-local, immutable variable 'a'
// = > ergo; I need the ability to detect if a slot is from a different named
//     scope

let val;
def some() {
  // [..]
  do(val);
}
val = 20;
some();

// 2 - when analyzing a function; build a list of non-local uses;
//     when calling a named function / "using" a function (
//     capturing it as a function object) then we check this list to determine
//     if the variable has been initialized


std::unordered_map<Ref<Block>, Assignment> _assign;

// Scope#top -- return top block
// Scope#at -- return blocks.at(-N)
// Block#contains -- does this block "contain" (or is) the passed block

let a;

{
  a = 20;
} // iter through assignments of this block; push non-local assignments
  // up one block

if cond
{
  a = 20;
}
else
{
  a = 20;
}
// at end of select -- iter through branch assignments
