class Token:

	def __init__(self, line, column, tokenType, lexeme):
		self.line = line
		self.column = column
		self.token_type = tokenType
		self.lexeme = str(lexeme)

	def printToken(self):
		print self.token_type