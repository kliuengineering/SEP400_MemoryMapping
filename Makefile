CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=main.cpp

WorkPack: $(FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o WorkPack

all: WorkPack