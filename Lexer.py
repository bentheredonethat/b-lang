from Token import Token
from LexemeTokenMapper import LexemeTokenMapper
import sys

class Lexer:

	current_line = 0
	current_column = 0
	tokens = []
	token_finder = LexemeTokenMapper()
	def __init__(self, input_file):
		self.filename = input_file

	def convertLexemeToToken(self, lexeme):
		# get matches
		# matches have syntax: 	("token_name": "...", "lexeme": lexeme) 
		match = self.token_finder.findMatchingToken(lexeme)
		# convert to token to return
		return Token(self.current_line, self.current_column, match["token_name"], lexeme)


	def run(self):
		
		lexemes = []

		 #Open a file
		lines_of_file = open(self.filename, "r").readlines()

		# make current char
		current_lexeme = ''

		# loop through file
		for line in lines_of_file:
			# update line and column count
			self.current_line += 1
			for current_char in line:
				self.current_column += 1
			# if current char is not space:
				if current_char != ' ':
			#	update current_lexeme
					current_lexeme += current_char
			# else current char is space:
				else:
			#	so add current lexeme to lexeme array
					lexemes.append(current_lexeme)
			#	reset current lexeme
					current_lexeme = ''

		# map each lexeme to its corresponding token
		tokens = map( self.convertLexemeToToken, lexemes)


		for i in tokens:
			i.printToken()
		return tokens



lex = Lexer("input.blang")
lex.run()