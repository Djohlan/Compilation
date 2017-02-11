CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb -g -std=c99

OBJ = analyseur_lexical.o util.o premiers.o suivants.o

all: test_yylex analyseur_syntaxique

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)
	
analyseur_syntaxique: analyseur_syntaxique.c $(OBJ)
	$(CC) $(CCFLAGS) -o analyseur_syntaxique analyseur_syntaxique.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c analyseur_lexical.c

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c analyseur_syntaxique.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
	- rm -f analyseur_syntaxique
