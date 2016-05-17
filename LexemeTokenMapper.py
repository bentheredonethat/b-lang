import re
import Token

class LexemeTokenMapper:
	
	# table of regex patterns that associate to token identifiers	
	token_patterns = [

		{
			"token_name" : "T_STRING",
			 "pattern" : "^(\"|\')[0-9a-zA-Z]*(\"|\'')" 
		}, 
		{
			"token_name" : "T_IDENTIFIER", 
			"pattern" : "[a-z]+[0-9a-zA-Z]*", 
		},
		{
			"token_name" : "T_INT",
			"pattern" : "[0-9]+"
		},
		{
			"token_name" : "T_DOUBLE",
			 "pattern" : "\d+\.\d+" 
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
			if len(current_pattern_hits) > 0:
		# priority can be inferred from index into table (higher is more important)
				matches.append({"priority" : index, "pattern_hits" : max(current_pattern_hits)})
 	
		return matches

	def filterMatches(self, matches):
			# arrange arrays for both priority and hits
			# make hit_array only contain max's for each hit array
			# return longest match

			if len(matches) == 0:
				return matches

			priority_arr = []
			hit_arr = []
			for i in matches:
				priority_arr.append(i["priority"])
				hit_arr.append(i["pattern_hits"])

			max_hit_idx = 0
			for i, elem in enumerate(hit_arr):
				if len(elem) >= hit_arr[max_hit_idx]:
					max_hit_idx = i

			max_pri_idx = 0
			for i, elem in enumerate(priority_arr):
				if elem >= hit_arr[max_pri_idx]:
					max_pri_idx = i

			if max_pri_idx == max_hit_idx:
				return matches[max_pri_idx]

			# in case of difference, favor with priority
			else:
				return matches[max_pri_idx]



	# given lexeme, look for any matches
	# if any matches are found that are same length match as lexeme:
	#	then return highest priority match
	# else return error token
	def findMatchingToken(self, lexeme):

		self.lexeme = lexeme

		if lexeme in self.KEYWORDS:
			return {"token_name" : "T_KEYWORD", "lexeme": self.lexeme }

		if lexeme in self.OPERATORS:
			return { "token_name" : "T_OPERATOR", "lexeme": self.lexeme } 

		match = self.getAllMatchesForLexeme()
		match = self.filterMatches(match)

		if len(match) == 0 :
			#no match so return error token
			return { "token_name" : "T_ERROR", "lexeme": self.lexeme }
		

		return { 
			"token_name" : self.token_patterns[match["priority"]]["token_name"], 
			"lexeme" : self.lexeme 
			}