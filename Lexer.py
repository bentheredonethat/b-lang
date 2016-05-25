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


		if lexeme[0] == "ENDOFLINE":
			return Token(lexeme[1], 0, "ENDOFLINE", "\n")
		else:	
			match = self.token_finder.findMatchingToken(lexeme[0])
			# convert to token to return
			# lexeme is array of lexeme, corresponding line and column
			return Token(lexeme[1], lexeme[2] - len(lexeme), match["token_name"], lexeme[0])


	def run(self):
		
		lexemes = []
		tokens = []
		 #Open a file
		lines_of_file = open(self.filename, "r").readlines()
		open_file = open("lo", "w+")
		# make current char
		current_lexeme = ''

		# loop through file
		for l in lines_of_file:
			# update line and column count
			self.current_line += 1
			line = l.replace("\n"," ")
			for current_char in line:
				self.current_column += 1
				if not current_char.isspace():
					#	update current_lexeme
						current_lexeme += current_char
					# else current char is space or newline:
				else:
					# if current char is neither space nor newline:
					#	so add current lexeme to lexeme array if not empty
					if len(current_lexeme) > 0:
						lexemes.append([current_lexeme, self.current_line, self.current_column])
					#	reset current lexeme
					current_lexeme = ''

			#reached end of line
			lexemes.append(["ENDOFLINE", self.current_column, self.current_line])


			self.current_column = 0
		# just in case there are lexemes that end at EOF
		if len(current_lexeme) > 0:
			lexemes.append([current_lexeme, self.current_column, self.current_line])




		# map each lexeme to its corresponding token
		tokens = map( self.convertLexemeToToken, lexemes)
		for index, elem in enumerate(tokens):
			open_file.write(elem.token_type +" "+str(lexemes[index][0])+  "\n")


		open_file.close()
		return tokens



lex = Lexer("input.blang")
lex.run()