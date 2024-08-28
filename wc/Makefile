
# ./src/*.c files
C_SOURCE = $(wildcard ./src/*.c)

# ./src/*.h files
H_SOURCE = $(wildcard ./src/*.h)

# ./src/*.o files
OBJ = $(C_SOURCE:.c=.o)


#
# Compilation and Linking
#
build: wc

wc: $(OBJ)
	gcc -o $@ $(OBJ)
	rm -rf $(OBJ)

./src/main.o: ./src/main.c $(H_SOURCE)
	gcc -o $@ $< -c

./src/wclib.o: ./src/wclib.c $(H_SOURCE)
	gcc -o $@ $< -c


test:
	./wc -l ./data/test.txt
	./wc -w ./data/test.txt
	./wc -m ./data/test.txt
	./wc ./data/test.txt


clean:
	rm -rf ./wc $(OBJ)