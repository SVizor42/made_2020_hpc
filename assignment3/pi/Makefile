CC=g++
CFLAGS=-fopenmp
EXECUTABLE=pi_calc.out
POINTS=

all: $(EXECUTABLE)

$(EXECUTABLE): omp_pi_calc.c
	$(CC) $(CFLAGS) omp_pi_calc.c -o $(EXECUTABLE)

run: 
	./$(EXECUTABLE) $(POINTS)

clean:
	rm -rf *.o $(EXECUTABLE)
