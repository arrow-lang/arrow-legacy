struct Box { value: int }
struct Container { box: Box }

let mutable cont_a = Container(Box(20));
let mutable cont_b = Container(cont_a.box);
let mutable cont_c = Container(Box(cont_a.box.value));

cont_a.box = cont_b.box;
cont_b.box.value = 30;
cont_a.box.value = cont_b.box.value;

def main() -> int {
  return cont_a.box.value;
}
