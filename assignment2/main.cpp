#include <assert.h>
#include <random>
#include <stdlib.h>
#include <sys/time.h>
//#include <time.h>
#include "matrix.h"

void getZeroMatrix(Matrix& m) {
	for (int i = 0; i < m.getRows(); i++)
		for (int j = 0; j < m.getColumns(); j++) {
			m[i][j] = 0;
		}
}

void getRandomMatrix(Matrix& m) {
	for (int i = 0; i < m.getRows(); i++)
		for (int j = 0; j < m.getColumns(); j++) {
            m[i][j] = rand() % 100 + 1;
        }
}

void runTest() {
}

int main(int argc, const char* argv[]) {
	srand(time(NULL));

//	Matrix size and number of tests
	size_t N = 512, iters = 10;
	if (argc > 1) {
		N = atoi(argv[1]);
		if (argc > 2) {
			iters = atoi(argv[2]);
		}
	}

	Matrix a(N, N), b(N, N);
    struct timeval start, end;
    double runTime = .0, avgTime = .0;

	getRandomMatrix(a);
	getRandomMatrix(b);

	std::cout << "Multiplying square matrices " << N << " x " << N << "...\n";
	for (size_t i = 0; i < iters; i++) {

    	gettimeofday(&start, NULL);
		Matrix c = a * b;
    	gettimeofday(&end, NULL);
    	runTime = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
		std::cout << "Running test #" << (i + 1) << ": time = " << runTime << " seconds \n";
        avgTime += runTime;

/*
		std::cout << "Print matrix a:" << std::endl;
		for (int i = 0; i < a.getRows(); i++) {
			for (int j = 0; j < a.getColumns(); j++) {
				std::cout << a[i][j] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << "Print matrix b:" << std::endl;
		for (int i = 0; i < b.getRows(); i++) {
			for (int j = 0; j < b.getColumns(); j++) {
				std::cout << b[i][j] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << "Print matrix c:" << std::endl;
		for (int i = 0; i < c.getRows(); i++) {
			for (int j = 0; j < c.getColumns(); j++) {
				std::cout << c[i][j] << ' ';
			}
			std::cout << std::endl;
		}
*/
	}

	avgTime /= iters;
    std::cout << "Average runtime is " << avgTime << " seconds\n";	

	return 0;
}
