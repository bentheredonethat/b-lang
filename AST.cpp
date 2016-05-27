#include <memory>


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
	  ExprAST* E;
	  ParenExprAST(ExprAST* e) : E(std::move(e)) {}
	  
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
			ExprAST * LHS;
			ExprAST* RHS;
	  BinaryExprAST(char op, ExprAST* lHS, ExprAST* rHS){
	  	Op = op;
	  	LHS = lHS;
	  	RHS = rHS;
	  }
	  char getOp()  { return Op; }
	  
};

/// StatementAST - Base class for all statement nodes.
class StatementAST {
public:
	virtual ~StatementAST() {}
};

/// AssignmentStaAST - Satement class for assignment statements
class AssignmentStaAST : public StatementAST {

	public:
		VariableExprAST * Var; // variable that is affected by assignment
		ExprAST * RHS; // right hand side of assignment statement
	  	AssignmentStaAST(VariableExprAST * var,ExprAST * rHS)
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

