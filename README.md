# b-lang
Compiler for language b-lang.

Sample on how to "make" a programming language. 


*Current Checklist* as of Friday May 27, 2016
- [x] Lexical Analysis
      * Yes, used Python to read from "input.blang" file
      * writes tokens into file called lo to be read from later
       TO DO
      - [ ] report column and line for each token
- [x] Syntax Analysis
-     Yes, will report error and parse correctly for the following:
      * expressions consisting of binary operator '+', nested parenthesis, variables, constants
      * statements that are either assignment or declaration
- [x] Generate Abstract Syntax Tree
-- bug fix: need to downcast appropriate pointers in Syntax Parser expression and statement functions
- [ ] Semantic Analysis
- [ ] Code Generation
- [ ] Optimize all the things!


__Compiler still in development, but if you still want to run it...__
* run "python Lexer.py" to tokenize your input.blang file
* Lexer.py produces file called "lo" that holds tokens with corresponding lexemes
* compile Analyzer(s) with "g++ Analyzer.cpp -o -std=c++11 -std=c++0x"
* run Analyzer executable in same directory that Lexer.py was run to analyze syntax.
