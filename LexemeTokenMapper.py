import re
import Token
class LexemeTokenMapper:
	
	# table of regex patterns that associate to token identifiers	
	token_patterns = [
		{
			"token_name" : "T_INT",
			"pattern" : "^\d*"
		},
		{
			"token_name" : "T_DOUBLE",
			 "pattern" : "^\d*\.\d*" 
		},
		{
			"token_name" : "T_STRING",
			 "pattern" : "^(\"|\')[0-9a-zA-Z]*(\"|\'')" 
		}, 
		{
			"token_name" : "T_IDENTIFIER", 
			"pattern" : "^[a-z][0-9a-zA-Z]*", 
		}
	]

	OPERATORS = {
		"/", "=", '*', '%', '+', '-', '-=', '+=', '<', '>', '>=', '<='
	}

	KEYWORDS = { 
	"if", "else", 
	"double", "int", 
	"while", "for",
	"string",
	"null",
	"break", "return", 
	"null", "new",
	"class", "interface", "extends", "implements", "extends"
	}
	def __init(self):
		self.lexeme = ""


	def getAllMatchesForLexeme(self):
		matches = []

		for index, current_pattern in enumerate(self.token_patterns):
		# try lexeme against each entity in token pattern
			current_pattern_hits = re.findall(current_pattern["pattern"], self.lexeme)
		# save each instance of matches from table with its associated priority

		# priority can be inferred from index into table (higher is more important)
			matches.append({"priority" : index, "pattern_hits" : current_pattern_hits})

		return matches

	def filterByLexeme(self, matches):		
			# save any match that exactly matches lexeme
			matches =  filter(lambda i: i == self.lexeme , matches)


	def filterMatchesByPriorty(self, matches):
		# only keep highest priority match list
		highest_priority = 0
		for i in matches:
			if i["priority"] > highest_priority:
				highest_priority = i["priority"]

		# in that list, return longest match
		for i in matches:
			if i["priority"] != highest_priority:
				matches.remove(i)

		return matches

	# given lexeme, look for any matches
	# if any matches are found that are same length match as lexeme:
	#	then return highest priority match
	# else return error token
	def findMatchingToken(self, lexeme):

		self.lexeme = lexeme

		matches = self.getAllMatchesForLexeme()
		matches = self.filterMatchesByPriorty(matches)
		matches = self.filterByLexeme(matches)


		if lexeme in self.KEYWORDS:
			return {"token_name" : "T_KEYWORD", "lexeme": self.lexeme }

		if lexeme in self.OPERATORS:
			return { "token_name" : "T_OPERATOR", "lexeme": self.lexeme } 

		if not (matches is not None and len(matches) == 0) :
			# no match so return error token
			return { "token_name" : "T_ERROR", "lexeme": self.lexeme }
		

		else:
			return matches