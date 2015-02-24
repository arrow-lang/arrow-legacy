#ifndef ARROW_AST_H
#define ARROW_AST_H 1

#include <deque>
#include <memory>
#include <iostream>
#include <stack>

#include <boost/property_tree/ptree.hpp>

#include "arrow/ast/nodes.hpp"

namespace arrow {
  namespace ast {

    struct Visitor {
      virtual ~Visitor() noexcept;

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

      virtual ~Show() noexcept;

      void show(std::ostream& os);

      virtual void visit(Node&) { }
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
