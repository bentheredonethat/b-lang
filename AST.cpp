#include <memory>
#include <string>
#include <unordered_map>
using namespace std;
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
};

// map from number to which pointer to downcast an expression to
std::unordered_map<int, std::string> ExpressionDowncastMap = {
	{0, "ExprAST"},
	{1, "NumberExprAST"},
	{2, "VariableExprAST"},
	{3, "StatementAST"},
	{4, "ParenExprAST"},
	{5, "BinaryExprAST"}
};

// ExpressionHolder - hold ExprAST object and number that represents which type of child
// class to downcast to
struct ExpressionHolder{
	ExprAST * expressionPtr;
	int whichType;

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
	  ParenExprAST(ExprAST* e){ E = e; }
	  
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


// map from number to which pointer to downcast an expression to
std::unordered_map<int, std::string> StatementDowncastMap = {
	{0, "DeclarationStaAST"},
	{1, "AssignmentStaAST"}
};

// StatementHolder - hold StatementAST object and number that represents which type of child
// class to downcast to
struct StatementHolder{
	StatementAST * statementPtr;
	int whichType;

};

/// AssignmentStaAST - Satement class for assignment statements
class AssignmentStaAST : public StatementAST {

	public:
		VariableExprAST * Var; // variable that is affected by assignment
		ExprAST * RHS; // right hand side of assignment statement
	  	AssignmentStaAST(VariableExprAST * var,ExprAST * rHS)
	  	 {
	  	 	Var = var;
	  	 	RHS = rHS;
	  	 }
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

