
#include <typeinfo>
#include <unordered_map>
#include <string>
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
	int tokenIndex = 0;
	Token *currentToken;

	void error(const char msg[]){
		cout << msg << endl;
	}

	int accept(string token_type) {
	    if (token_type == currentToken->token_type) {
	        getNextToken();
	        return 1;
	    }
	    return 0;
	}

	int expect(string token_type) {
	    if (accept(token_type))
	        return 1;
	    error("expect: unexpected symbol");
	    return 0;
	}

	// return 1 if successfully got next token
	// otherwise 0 for failure
	int getNextToken(){
		if (tokenIndex < (tokens.size() - 1)){
			tokenIndex++;
			currentToken = tokens[tokenIndex];
			cout << currentToken->lexeme << endl;

			return 1;
		}
		return 0;
	}

	// return 1 if successfully got next token
	// otherwise 0 for failure
	int getPrevToken(){
		if (tokenIndex > 0){
			tokenIndex--;
			currentToken = tokens[tokenIndex];
			return 1;
		}
		return 0;
	}

	struct ParseTree{
	};

	int type(int & success){ // ptr to entry in type table
		// check if type is in type vector

		if (currentToken->token_type != "TYPE"){
			success = 0;
			return 0;
		}
		success = 1;
		return 1;

		// vector<string>::iterator it;
		// it = find (typeTable.begin(), typeTable.end(), currentToken->lexeme);

		// // not in table	
		// if (it == typeTable.end()) {
		// 	typeTable.push_back(currentToken->lexeme);
		// }
		
	}
	int variable(int & success){  // ptr to entry in symbol table

		if (currentToken->token_type != "VAR"){
			success = 0;
			return 0;
		}
		success = 1;
		return 1;	
	}
	int identifier(int& success){ // description from variable or type
		if (currentToken->token_type != "INT" && currentToken->token_type != "VAR"){
			success = 0;
			return 0;
		}

		success = 1;
		return 1;	
	}

	int expression( int &success){
		// (E)
		// ^  <--- currently at '('
		// try expression inside
		// then if both expression inside is valid and after is ')' report success
		if (currentToken->lexeme == "("){
			getNextToken();
			int p = expression(success);
			if (!success)
				printf("syntax error: expression inside () is invalid\n" );
			else{
				if (currentToken->lexeme == ")"){
					getNextToken(); // skip over ) to next token
					return p; // stripped of ()
				}
			}
		}

		// variable
		if (currentToken->token_type == "VAR"){
			int p;
			variable(success);
			if (!success){
				printf("syntax error: variable inside expression is invalid\n" );
			}
			else getNextToken();
			// either way give back tree p
			return p;
		}

		// constant
		if (currentToken->token_type == "INT"){
			int p;
			identifier(success);
			if (!success){
				printf("syntax error: type inside expression is invalid\n" );
			}
			else getNextToken();
			// either way give back tree p
			return p;

		}

		// a + b
		int a = expression(success);
		if (success){
			int g = getNextToken();
			if (g && currentToken->token_type == "OPERATOR"){
				int b = expression(success);
				getNextToken();
				return (a && b) ? 1 : 0; 
			}
		}		
	}

	int declaration(int &success){
		// <declaration> ::= <type> <variable>


		type(success);
		if (!success) printf("syntax error: type at declaration invalid\n" );
		
		getNextToken();

		variable(success);
		if (!success) printf("syntax error: variable at declaration invalid\n" );
		
		success = 2;
		return 1; // 0 for error
	}

	int assignment(int &success){
		// <assignment> ::= <variable> = <expression>
		
		// check if current token is variable
		variable(success);
		if (!success){
			printf("syntax error: variable at assignment invalid\n" );
		}
		getNextToken();

		// nex token should be =
		char const *EQUALSIGN = "=";
		if ( currentToken->lexeme == EQUALSIGN){
			getNextToken();
		}

		int p;
		// some sequence of tokens should evaluate to a proper expression
		p = expression(success);

		// success should be modified in expression fn call
		return p;
	}

	int* statement(int& success){
		// <statement> ::= <declaration><endofline>| <assignment><endoflline>
		
		int * p;
		/* try for various statements:
			0 failure
			1 assignment success
			2 declaration success

		*/
		*p = assignment(success);
		if (success == 1){
			getNextToken(); // walk to newline token
			return p;
		}

		declaration( success);
	 	if (success == 2){
	 		getNextToken(); // walk to newline token
	 		success = 1;
	 		cout << "declaration valid, leaving statement()" << endl;
	 		*p = 1;
	 		return p;
	 	}  
		
		
		

		printf("error with syntax in statement\n");
		return 0;
	}

	int* program(int &success){
		// <program> :: <statement><program>
		int* programParseTree = new (int);
		if (programParseTree == NULL){
    		printf("Error: memory could not be allocated\n");
    		*programParseTree = 0;
    		return NULL;
    	}
		// 2 cases
		// if at end, return ptr to 0
		// else save statement in current int
		// and next int should point next token
		// at end of program!
		if (tokenIndex == tokens.size() - 1)
			return NULL;

		else{
			programParseTree = statement(success);
			if (!success){
				cout << "invalid statement" << endl;
				return NULL;
			}

			getNextToken();
			programParseTree++;
			programParseTree = program(success);
		}  
	}

public:
	SyntaxParser( std::vector<Token*> v){
		tokens = v;
		currentToken = v[0];

		for (int i = 0; i < tokens.size(); i++){
			cout << tokens[i]->lexeme << ", ";
		}
		cout << endl;
		for (int i = 0; i < tokens.size(); i++){
			cout << tokens[i]->token_type << ", ";
		}
		cout << endl;
	}
	int* getAST(int & success){
		return program(success);
	}
};