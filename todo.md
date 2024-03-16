1 program basic compiler(acwj)
  1.1 Lexer Done
  1.2 Scanner 
       1.2.1  (wrong precedence) D
       1.2.2  (correct precedence with changing grammar) D
       1.2.3  (correct precedence with Pratt parsing) D
  1.3 Codegen
      1.3.1  Basic codegen(match nodes with instructions, no floating point support) D
      1.3.2  Basic codegen(floating point support)
  1.4 Utils
      1.4.1 CMDLINE Options
            1.4.1.1 Basic support for print flags D
            1.4.1.2 Error on incorrect flags and make sure you take correct input file D
            1.4.1.3 Option for interpret or codegen
                    1.4.1.3 In codegen option for -S and -o compiler option
      1.4.2 Helper Functions
            1.4.2.1 

2 add advance techniques

3 testing and build
  3.1 build file
      3.1.1 Basic build file D
      3.1.2 Basic cmake support D
  3.2 test
      3.2.1 Basic test script D
      3.2.2 Add support for user args D
