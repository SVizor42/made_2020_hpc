CC=g++ -std=c++0x
OPT=-O3
DBGCFLAGS=
CFLAGS=-c ${OPT} ${DBGCFLAGS}
EXECUTABLE=matmul

all: $(EXECUTABLE)

$(EXECUTABLE): libmatrix.a
	$(CC) main.cpp libmatrix.a -o $(EXECUTABLE) 

libmatrix.a: matrix.cpp matrix.h
	$(CC) $(CFLAGS) matrix.cpp
	ar rc libmatrix.a matrix.o

run: 
	./$(EXECUTABLE)

clean:
	rm -rf *.o libmatrix.a $(EXECUTABLE) 
