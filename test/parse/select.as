if condition != false {
  statement;
  other_statement;
}

if condition > 10 {
  return 0;
} else if other_condition < 30 {
  return 30;
} else {
  return 20;
}

unless condition > 10 {
  return 0;
} else if other_condition < 30 {
  return 30;
} else unless other_condition < 30 {
  return 30;
} else {
  return 20;
}

let name = if condition { 40; } else { 30; };
let name = unless condition { 40; } else { 30; };
