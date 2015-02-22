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

    struct Break : Node {
      virtual void accept(Visitor& v);
    };

    struct Visitor {
      virtual void visit(Node&) { }
      virtual void visit(Module&) { }
      virtual void visit(Break&) { }
    };

    struct Show : Visitor {
    public:
      Show();

      void show(std::ostream& os);

      virtual void visit(Module& x);
      virtual void visit(Break& x);

    private:
      boost::property_tree::ptree& _el();

      boost::property_tree::ptree _tree;
      std::stack<boost::property_tree::ptree*> _ctx;

    };

  }
}

#endif // ARROW_AST_H
