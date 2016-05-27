# b-lang
Compiler for language b-lang.

Sample on how to "make" a programming language. 


*Current Checklist* as of Friday May 27, 2016
- [ x ] Lexical Analysis
      * Yes, used Python to read from "input.blang" file
      * writes tokens into file called lo to be read from later
      * TO DO
      - [ ] report column and line for each token
- [ x ] Syntax Analysis
-   * Yes, will report error and parse correctly for the following:
-     * expressions consisting of binary operator '+', nested parenthesis, variables, constants
-     * statements that are either assignment or declaration
- [ ] Generate Abstract Syntax Tree
- [ ] Semantic Analysis
- [ ] Code Generation
- [ ] Optimize all the things!
