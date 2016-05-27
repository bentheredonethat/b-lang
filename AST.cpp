#include <memory>
namespace {


/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
  double Val;

	public:
	  	NumberExprAST(double Val) : Val(Val) {}
  		int getVal() const { return Val; }
};

/// StringExprAST - Expression class for string literals like "abcd".
class StringExprAST : public ExprAST {
  	std::string Val;

	public:
	  	StringExprAST(std::string v){ Val = v; }
  		std::string getVal() const { return Val; }
};


/// ParenExprAST - Expression class for expressions within parenthises.
class ParenExprAST : public ExprAST {
  //std::unique_ptr<ExprAST> e;

	public:
	  std::unique_ptr<ExprAST> E;
	  ParenExprAST(std::unique_ptr<ExprAST> e) : E(std::move(e)) {}
	  
	  //std::unique_ptr<ExprAST> getExpression() const { return e; }
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;

	public:
	  VariableExprAST(const std::string &Name) : Name(Name) {}
	  std::string getName() const { return Name; }
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
	  char Op;
		//  std::unique_ptr<ExprAST> LHS, RHS;

		public:
			std::unique_ptr<ExprAST> LHS, RHS;
	  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
	                std::unique_ptr<ExprAST> RHS)
	    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
	     
	    char getOp()  { return Op; }
	    // std::unique_ptr<ExprAST> getLHS() const { return LHS; }
	    // std::unique_ptr<ExprAST> getRHS() const { return RHS; }
};

/// StatementAST - Base class for all statement nodes.
class StatementAST {
public:
	virtual ~StatementAST() {}
};

/// AssignmentStaAST - Satement class for assignment statements
class AssignmentStaAST : public StatementAST {

	public:
		std::unique_ptr<VariableExprAST> Var; // variable that is affected by assignment
		std::unique_ptr<ExprAST> RHS; // right hand side of assignment statement
	  	AssignmentStaAST(std::unique_ptr<VariableExprAST> var,std::unique_ptr<ExprAST> rHS)
	  	 : Var(std::move(var)), RHS(std::move(rHS)) {}
};

/// DeclarationStaAST - Satement class for declaration statements
class DeclarationStaAST : public StatementAST {
	// syntax for declaration is <Type><Var>
	public:
		std::string type, variable;

	  	DeclarationStaAST(std::string t, std::string v) {
	  		 type = t;
	  		 variable = v;
	  	}
};

}