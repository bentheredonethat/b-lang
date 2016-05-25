#include <string>

struct Token{
	int column;
	int line;
	std::string lexeme;
	std::string token_type;
};