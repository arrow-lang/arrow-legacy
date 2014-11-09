// #include <cstdio>
#include "arrow/tokenizer.hpp"

using namespace arrow;

int main() {
  // arrow < {input} > {output}
  // tokenize!

  auto tokenizer = Tokenizer("./test.as");

  for (;;) {
    auto tok = tokenizer.next();

    std::printf("%s: ", tok->span.to_string().c_str());

    if (tok->type == Type::Integer) {
      auto int_tok = std::static_pointer_cast<IntegerToken>(tok);
      std::printf("integer: %s (%d)\n", int_tok->text.c_str(), int_tok->base);
    } else {
      std::printf("%s\n", arrow::to_string(tok->type).c_str());
    }

    if (tok->type == arrow::Type::End) { break; }
  }
}
