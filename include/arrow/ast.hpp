#ifndef ARROW_AST_H
#define ARROW_AST_H 1

#include <deque>
#include <memory>
#include <iostream>
#include <stack>

#include <boost/property_tree/ptree.hpp>

namespace arrow {
  namespace ast {

    struct Visitor;

    struct Node {
      virtual void accept(Visitor& v);
    };

    struct Module : Node {
      virtual void accept(Visitor& v);

      std::deque<std::shared_ptr<Node>> sequence;
    };

    struct Integer : Node {
      Integer(const std::string& text, unsigned base);

      virtual void accept(Visitor& v);

      std::string text;
      unsigned base;
    };

    struct Float : Node {
      Float(const std::string& text);

      virtual void accept(Visitor& v);

      std::string text;
    };

    struct Boolean : Node {
      Boolean(bool value);

      virtual void accept(Visitor& v);

      bool value;
    };

    struct Unary : Node {
      Unary(std::shared_ptr<Node> operand);

      std::shared_ptr<Node> operand;
    };

    struct Promote : Unary {
      using Unary::Unary;

      virtual void accept(Visitor& v);
    };

    struct NegateNumeric : Unary {
      using Unary::Unary;

      virtual void accept(Visitor& v);
    };

    struct NegateLogical : Unary {
      using Unary::Unary;

      virtual void accept(Visitor& v);
    };

    struct NegateBitwise : Unary {
      using Unary::Unary;

      virtual void accept(Visitor& v);
    };

    struct Break : Node {
      virtual void accept(Visitor& v);
    };

    struct Visitor {
      virtual void visit(Node&) { }
      virtual void visit(Module&) { }
      virtual void visit(Break&) { }
      virtual void visit(Integer&) { }
      virtual void visit(Float&) { }
      virtual void visit(Boolean&) { }
      virtual void visit(Promote&) { }
      virtual void visit(NegateLogical&) { }
      virtual void visit(NegateBitwise&) { }
      virtual void visit(NegateNumeric&) { }
    };

    struct Show : Visitor {
    public:
      Show();

      void show(std::ostream& os);

      virtual void visit(Module&);
      virtual void visit(Break&);
      virtual void visit(Integer&);
      virtual void visit(Float&);
      virtual void visit(Boolean&);
      virtual void visit(Promote&);
      virtual void visit(NegateLogical&);
      virtual void visit(NegateBitwise&);
      virtual void visit(NegateNumeric&);

    private:
      boost::property_tree::ptree& _el();

      boost::property_tree::ptree _tree;
      std::stack<boost::property_tree::ptree*> _ctx;

    };

  }
}

#endif // ARROW_AST_H
