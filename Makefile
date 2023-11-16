
default : build execute

build : 
	clang++ lex.cpp -Wall  -o bin/aclj

execute : 
	bin/./aclj

