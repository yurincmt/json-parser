
# $@
# $<
# $^

C_SOURCE = $(wildcard ./src/*.c)

H_SOURCE = $(wildcard ./src/*.h)

OBJ = $(C_SOURCE:.c=.o)

# 
# Compilation and Linking
# 
build: jparser

jparser: $(OBJ)
	gcc -o $@ $(OBJ)
	rm -rf $(OBJ)

./src/parsermain.o: ./src/parsermain.c $(H_SOURCE)
	gcc -o $@ $< -c

./src/parser.o: ./src/parser.c $(H_SOURCE)
	gcc -o $@ $< -c

./src/lex.o: ./src/lex.c $(H_SOURCE)
	gcc -o $@ $< -c


#
# Debugging binary
#
debug: ./src/db

./src/db: $(OBJ)
	gcc -g -o $@ $(OBJ)
	rm -rf $(OBJ)

./src/parsermain.o: ./src/parsermain.c $(H_SOURCE)
	gcc -g -o $@ $< -c

./src/parser.o: ./src/parser.c $(H_SOURCE)
	gcc -g -o $@ $< -c

./src/lex.o: ./src/lex.c $(H_SOURCE)
	gcc -g -o $@ $< -c


test:

testerror:
	

clean:
	rm -rf ./jparser $(OBJ)
	rm -rf ./src/a.out ./src/lex ./src/parser ./src/parsermain