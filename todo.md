1. program basic compiler([acwj](https://github.com/DoctorWkt/acwj/ "a compiler writing journey") inspired)
   1. Lexer
       1. Others
          - [X] 1. arithmetic tokens(+,-,*,/)
          - [X] 2. integral and floating point numbers
          - [X] 3. variables

       2. Statements
          - [X] 1. assignment
          - [X] 2. ifelse
          - [X] 3. while and dowhile
	
   2. Scanner
        - [X] 1. wrong precedence
        - [X] 2. correct precedence with changing grammar
        - [X] 3. correct precedence with Pratt parsing

   3. Codegen
         - [X] 1. match nodes with instructions(naive)
         - [ ] 2. floating point support
   
   4. Error Handling
       1. Lex error
          - [X] 1. error on wrong tokens
       2. Syntax error
          - [X] 1. error on not following grammar
       3. Source/Location pointing
          - [ ] 1. line number support   
   
   5. Utils
       1. CMDLINE Options
          - [X] 1. Basic support for print flags
          - [X] 2. Error on incorrect flags and make sure you take correct input file
          - [ ] 3. -S and -o flags
      2. Helper Functions
          - [X] 1. Print tokens
          - [X] 2. Print ast(without precedence)
   
   6. Naive optimisations( separate branch, please ignore these names)
      1. -O
         - [X] 1. Interpret comparisons
      2. -Onobranch
         - [ ]  2. Single node if else

2. add advance techniques

3. testing and build
   1. build file
       - [X] 1. Basic build file
       - [X] 2. Basic cmake support
   2. test
       - [X] 1. Basic test script
       - [X] 2. user args
