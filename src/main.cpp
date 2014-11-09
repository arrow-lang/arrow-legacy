// #include <cstdio>
#include "arrow/tokenizer.hpp"

int main() {
  // arrow < {input} > {output}
  // tokenize!

  auto tokenizer = arrow::Tokenizer("./test.as");

  for (;;) {
    auto tok = tokenizer.next();

    std::printf("%s: ", tok.span.to_string().c_str());
    std::printf("note: ");
    std::printf("%s\n", arrow::to_string(tok.type).c_str());

    if (tok.type == arrow::Type::End) { break; }
  }
}
