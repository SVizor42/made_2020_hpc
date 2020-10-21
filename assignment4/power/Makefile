CC=g++
CFLAGS=-fopenmp
OPT=-O3
EXECUTABLE=matrix_power.out
SIZE=
POWER=

all: $(EXECUTABLE)

$(EXECUTABLE): matrix_power.c
	$(CC) $(CFLAGS) $(OPT) matrix_power.c -o $(EXECUTABLE)

run: 
	./$(EXECUTABLE) $(SIZE) $(POWER)

clean:
	rm -rf *.o $(EXECUTABLE)
