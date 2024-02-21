build : 
	clang++ main.cpp lex.cpp tree.cpp expr.cpp expr2.cpp  interpret.cpp helper.cpp -Wall --std=c++17 -g  -o aclj
