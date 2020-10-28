export -g

TESTFILE = "sourcecode.txt"

compileAll : grammar.o tokenStream.o parseTree.o stack.o typeExpressionTable.o printTypeExpressionTable.o printParseTree.o main.o
	gcc grammar.o tokenStream.o parseTree.o stack.o typeExpressionTable.o printTypeExpressionTable.o printParseTree.o main.o -o runprogram

grammar.o : grammar.c grammar.h
	gcc -c grammar.c

tokenStream.o : tokenStream.c tokenStream.h
	gcc -c tokenStream.c

stack.o : stack.c stack.h
	gcc -c stack.c

parseTree.o : parseTree.c stack.h parseTree.h
	gcc -c parseTree.c 

typeExpressionTable.o : typeExpressionTable.c typeExpressionTable.h
	gcc -c typeExpressionTable.c

printParseTree.o : printParseTree.c printParseTree.h
	gcc -c printParseTree.c

printTypeExpressionTable.o : printTypeExpressionTable.c printTypeExpressionTable.h
	gcc -c printTypeExpressionTable.c

main.o : main.c printParseTree.h
	gcc -c main.c

clean : 
	rm -rf $(wildcard *.o)
	rm -rf typeExpressionTableOutput.txt
	rm -rf runprogram

run: runprogram
	./runprogram ${TESTFILE}
