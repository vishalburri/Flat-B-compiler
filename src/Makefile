bcc:	parser.tab.c lex.yy.c methods.cpp classes.h
	g++ -w  -std=c++11 parser.tab.c lex.yy.c methods.cpp `llvm-config-3.8 --cxxflags --cppflags mcjit native --ldflags --system-libs --libs core` -lfl   -ltinfo -lpthread -ldl  -o bcc

parser.tab.c: parser.y
	bison -d parser.y
parser.tab.h: parser.y
	bison -d parser.y
lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

.PHONY: clean
clean:
	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true
