#ifndef ARROW_AST_NODES_H
#define ARROW_AST_NODES_H 1

#include <deque>
#include <string>
#include <memory>

namespace arrow {
  namespace ast {

    struct Visitor;

    struct Node {
      virtual ~Node() noexcept;

      virtual void accept(Visitor& v);
    };

    struct Module : Node {
      Module();

      virtual ~Module() noexcept;

      virtual void accept(Visitor& v);

      std::deque<std::shared_ptr<Node>> sequence;
    };

    struct Integer : Node {
      Integer(const std::string& text, unsigned base);

      virtual ~Integer() noexcept;

      virtual void accept(Visitor& v);

      std::string text;
      unsigned base;
    };

    struct Float : Node {
      Float(const std::string& text);

      virtual ~Float() noexcept;

      virtual void accept(Visitor& v);

      std::string text;
    };

    struct Boolean : Node {
      Boolean(bool value);

      virtual ~Boolean() noexcept;

      virtual void accept(Visitor& v);

      bool value;
    };

    struct Unary : Node {
      Unary(std::shared_ptr<Node> operand);

      virtual ~Unary() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Node> operand;
    };

    struct Promote : Unary {
      using Unary::Unary;

      virtual ~Promote() noexcept;

      virtual void accept(Visitor& v);
    };

    struct NegateNumeric : Unary {
      using Unary::Unary;

      virtual ~NegateNumeric() noexcept;

      virtual void accept(Visitor& v);
    };

    struct NegateLogical : Unary {
      using Unary::Unary;

      virtual ~NegateLogical() noexcept;

      virtual void accept(Visitor& v);
    };

    struct NegateBitwise : Unary {
      using Unary::Unary;

      virtual ~NegateBitwise() noexcept;

      virtual void accept(Visitor& v);
    };

    struct Break : Node {
      virtual ~Break() noexcept;

      virtual void accept(Visitor& v);
    };

  }
}

#endif // ARROW_AST_NODES_H
