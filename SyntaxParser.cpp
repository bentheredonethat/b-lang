
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <stdio.h> 
#include <stack>
#include <algorithm>    // std::find
#include "AST.cpp"
using namespace std;

class SyntaxParser{ 

public:
	// hold types for program
	vector<string> typeTable;

	// hold variable names, map to locations in types table
	unordered_map<Token*,vector<string>::iterator> symbolTable;

	// hold sequence of statements that comprise program
	vector<StatementAST> programAST;



private:
	std::vector<Token*> tokens; 
	int tokenIndex; // hold current location in tokens that is being parsed

	Token * currentToken(){
		return tokens[tokenIndex];
	}

	// return 1 if successfully got next token
	// otherwise 0 for failure
	int getNextToken(){
		if (tokenIndex < tokens.size() - 1){
			tokenIndex += 1;
			cout << "token index " << tokenIndex << "\t\t" << currentToken()->lexeme << endl;
			return 1;
		}
		
		cout << "no more tokens\n";
		return 0;
	}

	// look ahead at next token
	Token * peek(){
		if (tokenIndex < (tokens.size() - 1)){
			return tokens[tokenIndex+1];
			
		}
		cout << "peeked out of bounds of tokens\n";
		return NULL;
		exit(EXIT_FAILURE);
	}


	// check if current token is a type token
	int type(){ // ptr to entry in type table
		// check if type is in type vector
		return currentToken()->token_type == "TYPE";

		// vector<string>::iterator it;
		// it = find (typeTable.begin(), typeTable.end(), currentToken->lexeme);

		// // not in table	
		// if (it == typeTable.end()) {
		// 	typeTable.push_back(currentToken->lexeme);
		// }
	}

	// check if current token is a var token
	VariableExprAST * variable(){  
		//ptr to entry in symbol table
		if (currentToken()->token_type == "VAR"){
			VariableExprAST * a = new VariableExprAST(currentToken()->lexeme);
			return a;
		}
		return NULL;
{

}
	}

	// check if current token is a 'literal' token
	NumberExprAST * literal(){ 
		// description from variable or string
		if (currentToken()->token_type == "INT"){
			NumberExprAST * a = new NumberExprAST(stod(currentToken()->lexeme));
			return a;
		}
		return NULL;
	}

	// return whether current token and a subsequent array evaluates to an expression
	// if so return true. if not, then wind back to where the function started in token array
	ExprAST* expression( ){
		//cout << "gf\n";
		int i = tokenIndex;

		bool parenExpression = 1;

		// BUG how to handle end of paren expression ')'
		//cout << currentToken()->lexeme << endl;		
		ExprAST * p;
		if (currentToken()->token_type == "("){
			printf("parsing parenExpression\n");
			getNextToken(); // eat (
			p = new ParenExprAST ( expression() ) ;
			parenExpression &= (p ? 1 : 0) ; // if p is not null (i.e. valid). then say it was success
			parenExpression &= (peek()->token_type == ")"); 
			getNextToken(); // go to )
			return p;
		}
		
		ExprAST * v = variable();
		ExprAST * constant = literal();

		// handle possible binary expression E + E
		if (v || constant || parenExpression){
			if (peek()->token_type == "OPERATOR"){ // if binary expression then check
				getNextToken(); // go to op
				getNextToken(); // eat op

				// try LHS expression
				ExprAST * RHS = expression();

				// figure out which is RHS
				ExprAST * LHS = (v ? v : constant);
				LHS = (p ? p : constant);

				// if both left and right side of binary expr are valid, then return it
				if (LHS && RHS) return new BinaryExprAST('+', LHS, RHS );
				// otherwise dont
				return NULL;

			}
			cout << "valid expression\n";

			// valid expression, but not binary, so figure out which type of expression
			ExprAST * ret = (v ? v : constant);
			ret = (p ? p : constant);
			
		}
		//cout << "error in expression\n";
		tokenIndex = i; // wind it back
		cout << "after winding back in expression, current token is " << currentToken()->lexeme << endl;
		return NULL;
	}

	// return whether current token and a subsequent array evaluates to an declaration
	// if so return declaration object. if not, then wind back to where the function started in token array
	DeclarationStaAST * declaration(){
		// <declaration> ::= <type> <variable>
		int i = tokenIndex;

		int a = type();
		string typeValue;
		if (a) typeValue = currentToken()->lexeme;

		getNextToken(); // eat type
		VariableExprAST *b = variable();
		int success = a && b;
		if (!success){ // wind it back
			//cout << "error in declaration\n";
			// should be back at beginning
			tokenIndex = i; // wind back
			cout << "after winding back in declaration, current token is " << currentToken()->lexeme << endl;
			return NULL;
		}
		cout << "valid declaration\n";
		// current token is variable in this case
		return new DeclarationStaAST(typeValue, currentToken()->lexeme);
	}

	// return whether current token and a subsequent array evaluates to an assignment
	// if so return assignment statement object. if not, then wind back to where the function started in token array
	AssignmentStaAST * assignment(){
		// <assignment> ::= <variable> = <expression>
		int i = tokenIndex; // Save current index
		
		// if <variable><=><E>
		VariableExprAST * var = variable();
		int isVar = var ? 1 : 0;
		getNextToken(); // eat variable
		int isAssigned =  currentToken()->lexeme == "=" ;
		getNextToken(); // eat =
		ExprAST * expr = expression();
		int isExpression =  expr ? 1 : 0;
		if (isVar && isAssigned && isExpression){
			cout << "valid assignment\n";
			return new AssignmentStaAST(var, expr);
		}
		
		tokenIndex = i; // wind back
		cout << "after winding back in assignment, current token is " << currentToken()->lexeme << endl;
		return NULL;
		
	}

// disable optimizations in here for debugging
// to undo, remove pragmas
#pragma GCC push_options
#pragma GCC optimize ("O0")
	// should evaluate statement and point current token to eol after end of statement
	StatementAST * statement(){

		// <statement> ::= <declaration><endofline>| <assignment><endoflline> 
		StatementAST * a = declaration();
		StatementAST * b = assignment();

		if ( a || b ){
		// 	Check for EOL
			if ( peek()->token_type == "ENDOFLINE"){
				cout << "got endofline after valid statement that ends with  " << currentToken()->lexeme << endl;
				getNextToken(); // get to EOL
				return a ? a : b; // figure out which type of statement to return
			}		
		}
		// if not valid statement, report error and parse next statement

		// Read to next EOL
		while (peek()->token_type != "ENDOFLINE") getNextToken();
		cout << "invalid statement\n";
		return NULL; // Return false
	}
#pragma GCC pop_options


	// using tail recursion, this function recurses toward end of tokens and uses
	// recursive descent to parse tokens into collection of AST's representing statements
	std::vector<StatementAST*> program(){
		// <program> :: <statement><program>
		
		// hold current (and rest of) statement evaluations
		std::vector<StatementAST*> v;

		// if already last token, return 
		if (tokenIndex == tokens.size() - 1)
			return v;


		else{
			StatementAST* p = statement(); // evaluate for current location in tokens
			// report error or success
			cout << "this past statement was " << (p ? "valid" : "not valid") << endl;

			v.push_back ( p ); // save report


			// if there are any tokens left, recurse on next token

			Token * notDone = peek(); // see if there is a next statement
			if (notDone){ 
				getNextToken(); // eat EOL from end of last statement
				cout << "about to try new statement that starts with " << currentToken()->lexeme << endl;
			
				// before recursing, add rest of program to report	
				std::vector<StatementAST*> restOfProgram = program();
				v.insert(v.end(), restOfProgram.begin(), restOfProgram.end()); 
			}
			else cout << "done syntax parsing\n";
			// return to what should be end of report via tail recursion
			return v;
		}  
	}

public:
	SyntaxParser( std::vector<Token*> v){
		tokens = v;
		tokenIndex = 0; // current token should start at beginnig

	}
	std::vector<StatementAST*> getAST(){
		return program(); // currently AST is not yet implemented, just reports whether syntax is correct
	}
};