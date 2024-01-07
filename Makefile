build : 
	clang++ main.cpp lex.cpp -Wall --std=c++17 -g  -o aclj

install : 
	source init.sh	

test   :
	source test.sh
