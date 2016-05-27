
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <stdio.h> 
#include <stack>
#include <algorithm>    // std::find
using namespace std;

class SyntaxParser{ 

public:
	// hold types for program
	vector<string> typeTable;

	// hold variable names, map to locations in types table
	unordered_map<Token*,vector<string>::iterator> symbolTable;


private:
	std::vector<Token*> tokens;
	int tokenIndex;

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

	Token * peek(){
		if (tokenIndex < (tokens.size() - 1)){
			return tokens[tokenIndex+1];
			
		}
		cout << "peeked out of bounds of tokens\n";
		return NULL;
		exit(EXIT_FAILURE);
	}

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
	int variable(){  
		//ptr to entry in symbol table
		return currentToken()->token_type == "VAR";
	}
	int constant(){ 
		// description from variable or string
		return (currentToken()->token_type == "INT");
	}


	int expression( ){
		//cout << "gf\n";
		int i = tokenIndex;

		bool parenExpression = 1;

		// BUG how to handle end of paren expression ')'
		//cout << currentToken()->lexeme << endl;		

		if (currentToken()->token_type == "("){
			printf("parsing parenExpression\n");
			getNextToken(); // eat (
			parenExpression &= expression() ;
			parenExpression &= (peek()->token_type == ")"); 
			getNextToken(); // go to )
			return parenExpression;
		}
		
		// handle possible binary expression E + E
		if (variable() || constant() || parenExpression){
			if (peek()->token_type == "OPERATOR"){ // if binary expression then check
				getNextToken(); // go to op
				getNextToken(); // eat op
				return expression();

			}
			cout << "valid expression\n";
			return 1;
		}
		//cout << "error in expression\n";
		tokenIndex = i; // wind it back
		cout << "after winding back in expression, current token is " << currentToken()->lexeme << endl;
		return 0;
	}

	bool declaration(){
		// <declaration> ::= <type> <variable>
		int i = tokenIndex;

		int a = type();
		getNextToken(); // eat type
		int b = variable();
		int success = a & b;
		if (!success){ // wind it back
			//cout << "error in declaration\n";
			// should be back at beginning
			tokenIndex = i; // wind back
			cout << "after winding back in declaration, current token is " << currentToken()->lexeme << endl;
			return false;
		}
		cout << "valid declaration\n";
		return true;
	}


	bool assignment(){
		// <assignment> ::= <variable> = <expression>
		int i = tokenIndex; // Save current index
		
		// if <variable><=><E>
		int isVar = variable();
		getNextToken(); // eat variable
		int isAssigned =  currentToken()->lexeme == "=" ;
		getNextToken(); // eat =
		int isExpression =  expression();
		if (isVar && isAssigned && isExpression){
			cout << "valid assignment\n";
			return true;
		}
		
		tokenIndex = i; // wind back
		cout << "after winding back in assignment, current token is " << currentToken()->lexeme << endl;
		return false;
		
	}

// disable optimizations in here for debugging
// to undo, remove pragmas
#pragma GCC push_options
#pragma GCC optimize ("O0")
	// should evaluate statement and put point current token to eol at end of statement
	int statement(){




		// <statement> ::= <declaration><endofline>| <assignment><endoflline> | <expression><endofline>
		if (assignment() || declaration() || expression() ){
		// 	Check for EOL
			if ( peek()->token_type == "ENDOFLINE"){
				cout << "got endofline after valid statement that ends with  " << currentToken()->lexeme << endl;
				getNextToken(); // get to EOL
				return 1;
			}		
		}

		// Read to next EOL
		while (peek()->token_type != "ENDOFLINE") getNextToken();
		cout << "invalid statement\n";
		return 0; // Return false
	}
#pragma GCC pop_options

	std::vector<int> program(){
		// <program> :: <statement><program>
		std::vector<int> v;

		if (tokenIndex == tokens.size() - 1)
			return v;

		int* t = new (int);
		if (t == NULL){
    		printf("Error: memory could not be allocated\n");
    		v.push_back(0);
    		return v;
    	}

		else{
			int p = statement();
			cout << "this past statement was " << (p? "valid" : "not valid") << endl;

			v.push_back ( p );


			Token * notDone = peek(); // see if there is a next statement
			if (notDone){ 
				getNextToken(); // eat EOL from end of last statement
				cout << "about to try new statement that starts with " << currentToken()->lexeme << endl;
				
				std::vector<int> restOfProgram = program();
				v.insert(v.end(), restOfProgram.begin(), restOfProgram.end()); 
			}
			else cout << "done syntax parsing\n";
			return v;
		}  
	}

public:
	SyntaxParser( std::vector<Token*> v){
		tokens = v;
		tokenIndex = 0;

	}
	std::vector<int> getAST(){
		return program();
	}
};