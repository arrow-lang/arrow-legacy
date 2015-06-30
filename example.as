
// Pattern

// - Identifier (catch-all and bind)
match value { case x => .. }

// - Wildcard (catch-all and don't bind)
match value { case _ => }

// - Literal
match value { case false => { .. } case true => { .. } }
match value { case "hello" => { .. } case 40 => { .. } }
match value {
  case 0 => { .. }
  case 1 => { .. }
  case 2 => { .. }
  case 3 => { .. }
  case 4 => { .. }
  case 5 => { .. }
  case 6, 7 => { .. }
  // case 8..100 => { .. }
  case _ => { .. }
}

export extern let name: int;

// - Tuple (catch, unpack, bind, ignore)
match value { case (x, y) => .. }
match value { case (x, _) => .. }
match value { case (40, y) => .. }

// - Guard
match value { case x if x < 40 => .. }

// "Binding" pattern restricts patterns to just
// identifier, tuple (of identifiers), and "_"


let main() -> int {
  let file = File.open("./std.txt").or_else((err) => {
    return -1;  // Returns from lambda
  });

  let text = file.read().or_else(|| {
    return -1;  // Returns from main
  })
}
