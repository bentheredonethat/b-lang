import re
import Token

class LexemeTokenMapper:
	
	# table of regex patterns that associate to token identifiers	
	token_patterns = [

		{
			"token_name" : "STRING",
			 "pattern" : "(\".+?\")|(\'.+?\')"
		}, 
		{
			"token_name" : "VAR", 
			"pattern" : "[a-z]+[0-9a-zA-Z]*", 
		},
		{
			"token_name" : "INT",
			"pattern" : "[0-9]+"
		}
		,{
			"token_name" : "(",
			 "pattern" : "(\()" 
		},{
			"token_name" : ")",
			 "pattern" : "(\))" 
		},{
			"token_name" : "ASSIGNMENT_OP",
			 "pattern" : "(=)" 
		}
	]

	OPERATORS = {
		'+'
	}

	TYPES = {
		"double", "int", "string"
	}

	KEYWORDS = { 
		"if", "else", 
		"while", "for",
		"null",
		"break", "return", 
		"null", "new"
	}

	def __init(self):
		self.lexeme = ""


	def getAllMatchesForLexeme(self):
		matches = []

		for index, current_pattern in enumerate(self.token_patterns):
		# try lexeme against each entity in token pattern
			current_pattern_hits = re.findall(current_pattern["pattern"], self.lexeme)
		# save each instance of matches from table with its associated priority
			if len(current_pattern_hits) > 0:
		# priority can be inferred from index into table (higher is more important)
				matches.append({"priority" : index, "pattern_hits" : max(current_pattern_hits)})
		return matches

	def filterMatches(self, matches):
			# filter matches based on longest match

			if len(matches) == 0:
				return matches

			hit_arr = []
			for i in matches:
				hit_arr.append(i["pattern_hits"])

			max_hit_idx = 0
			for i, elem in enumerate(hit_arr):
				if len(elem) == len(self.lexeme):
					return matches[i]



	# given lexeme, look for any matches
	# if any matches are found that are same length match as lexeme:
	#	then return best match
	# else return error token
	def findMatchingToken(self, lexeme):

		self.lexeme = lexeme

		if lexeme in self.KEYWORDS:
			return {"token_name" : "KEYWORD", "lexeme": self.lexeme }

		if lexeme in self.TYPES:
			return {"token_name" : "TYPE", "lexeme": self.lexeme }

		if lexeme in self.OPERATORS:
			return { "token_name" : "OPERATOR", "lexeme": self.lexeme } 

		match = self.getAllMatchesForLexeme()
		match = self.filterMatches(match)

		if match is not None:
			if len(match) > 0:
				return { 
				"token_name" : self.token_patterns[match["priority"]]["token_name"], 
				"lexeme" : self.lexeme 
				}

		#no match so return error token
		return { "token_name" : "T_ERROR", "lexeme": self.lexeme }
		

		