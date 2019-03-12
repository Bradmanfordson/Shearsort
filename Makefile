CC=g++
CFLAGS=-Wall -Werror 

BIN=main
EXE=shearsort
EXT=cpp

make: $(BIN).$(EXT)
	$(CC) $(BIN).$(EXT) $(CFLAGS) -o $(EXE)

clean:
	rm -f $(EXE)