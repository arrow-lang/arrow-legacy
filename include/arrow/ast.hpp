#ifndef ARROW_AST_H
#define ARROW_AST_H 1

#include <deque>
#include <memory>
#include <iostream>

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

    struct Visitor {
      virtual void visit(Node&) { }
      virtual void visit(Module&) { }
    };

    struct Show : Visitor {
    public:
      Show();

      void show(std::ostream& os);

      virtual void visit(Module& x);

    private:
      boost::property_tree::ptree _tree;
      boost::property_tree::ptree& _el;

    };

  }
}

#endif // ARROW_AST_H
