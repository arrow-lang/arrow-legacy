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

    struct TextNode : Node {
      TextNode(const std::string& text);

      virtual ~TextNode() noexcept;

      virtual void accept(Visitor& v);

      std::string text;
    };

    struct Identifier : TextNode {
      using TextNode::TextNode;

      virtual ~Identifier() noexcept;

      virtual void accept(Visitor& v);
    };

    struct Integer : TextNode {
      Integer(const std::string& text, unsigned base);

      virtual ~Integer() noexcept;

      virtual void accept(Visitor& v);

      unsigned base;
    };

    struct Float : TextNode {
      using TextNode::TextNode;

      virtual ~Float() noexcept;

      virtual void accept(Visitor& v);
    };

    struct Boolean : Node {
      Boolean(bool value);

      virtual ~Boolean() noexcept;

      virtual void accept(Visitor& v);

      bool value;
    };

    struct String : Node {
      String(const std::vector<std::uint8_t>& bytes);

      virtual ~String() noexcept;

      virtual void accept(Visitor& v);

      std::string text() {
        return std::string(
          reinterpret_cast<char*>(bytes.data()),
          bytes.size());
      }

      std::vector<std::uint8_t> bytes;
    };

    struct Return : Node {
      Return(std::shared_ptr<Node> expression);

      virtual ~Return() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Node> expression;
    };

    struct Unary : Node {
      Unary(std::shared_ptr<Node> operand);

      virtual ~Unary() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Node> operand;
    };

    #define UNARY_DEFINE(N) \
      struct N : Unary { \
        using Unary::Unary; \
        virtual ~N() noexcept; \
        virtual void accept(Visitor& v); \
      }

    UNARY_DEFINE(Promote);
    UNARY_DEFINE(NegateNumeric);
    UNARY_DEFINE(NegateLogical);
    UNARY_DEFINE(NegateBit);

    #undef UNARY_DEFINE

    struct Break : Node {
      virtual ~Break() noexcept;

      virtual void accept(Visitor& v);
    };

    struct Binary : Node {
      Binary(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs);

      virtual ~Binary() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Node> lhs;
      std::shared_ptr<Node> rhs;
    };

    #define BINARY_DEFINE(N) \
      struct N : Binary { \
        using Binary::Binary; \
        virtual ~N() noexcept; \
        virtual void accept(Visitor& v); \
      }

    BINARY_DEFINE(Assign);
    BINARY_DEFINE(AssignAdd);
    BINARY_DEFINE(AssignSub);
    BINARY_DEFINE(AssignMul);
    BINARY_DEFINE(AssignDiv);
    BINARY_DEFINE(AssignMod);
    BINARY_DEFINE(AssignBitAnd);
    BINARY_DEFINE(AssignBitXor);
    BINARY_DEFINE(AssignBitOr);
    BINARY_DEFINE(And);
    BINARY_DEFINE(Or);
    BINARY_DEFINE(EqualTo);
    BINARY_DEFINE(NotEqualTo);
    BINARY_DEFINE(LessThan);
    BINARY_DEFINE(LessThanOrEqualTo);
    BINARY_DEFINE(GreaterThanOrEqualTo);
    BINARY_DEFINE(GreaterThan);
    BINARY_DEFINE(BitAnd);
    BINARY_DEFINE(BitXor);
    BINARY_DEFINE(BitOr);
    BINARY_DEFINE(Add);
    BINARY_DEFINE(Sub);
    BINARY_DEFINE(Mul);
    BINARY_DEFINE(Div);
    BINARY_DEFINE(Mod);

    #undef BINARY_DEFINE

    struct Function : Node {
      Function(std::shared_ptr<Identifier> name);

      virtual ~Function() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Identifier> name;
      std::deque<std::shared_ptr<Node>> sequence;
    };

    struct Call : Node {
      Call(std::shared_ptr<Node> expression);

      virtual ~Call();

      virtual void accept(Visitor& v);

      std::shared_ptr<Node> expression;

    };

    struct Slot : Node {
      Slot(
        std::shared_ptr<Identifier> name,
        std::shared_ptr<Node> type,
        std::shared_ptr<Node> initializer = nullptr);

      virtual ~Slot() noexcept;

      virtual void accept(Visitor& v);

      std::shared_ptr<Identifier> name;
      std::shared_ptr<Node> type;
      std::shared_ptr<Node> initializer;
    };

  }
}

#endif // ARROW_AST_NODES_H
