#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

const int nThreads = 4;
unsigned int seeds[nThreads];

void seedThreads() {
    int my_thread_id;
    unsigned int seed;
    #pragma omp parallel private (seed, my_thread_id)
    {
        my_thread_id = omp_get_thread_num();
        unsigned int seed = (unsigned) time(NULL);
        seeds[my_thread_id] = (seed & 0xFFFFFFF0) | (my_thread_id + 1);
    }
}
 
void zeroMatrix(double *A, const size_t N) {
    #pragma omp parallel for
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++) {
            A[i * N + j] = 0.0;
        }
}
 
void randomMatrix(double *A, const size_t N) {
    srand(time(NULL));
    #pragma omp parallel for 
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++) {
            A[i * N + j] = (double) rand() / RAND_MAX;
        }
}
 
void calcMatMul(double *A, double *B, double *C, const size_t N) {
    size_t i, j, k, offset_i, offset_k;
    #pragma omp parallel for private(i, j, k, offset_i, offset_k)
    for (k = 0; k < N; k++) {
    	offset_k = k * N;
        for (i = 0; i < N; i++) {
        	offset_i = i * N;
            for (j = 0; j < N; j++) {
                C[offset_i + j] += A[offset_i + k] * B[offset_k + j];
          	}
      	}
  	}
}
 
double* matrixBinPow(double *Adj_m, const size_t N, const size_t power) {    
    size_t i, j, k;

    double* result = (double *) malloc(N * N * sizeof(double));	    
	zeroMatrix(&result[0], N);
  
    if (power == 0) {
    	#pragma omp parallel for private(i, j)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (i == j) {
                    result[i * N + j] = 1.0;
             	}
    }
    else {
     	double* temp = (double *) malloc(N * N * sizeof(double)); 	
     	zeroMatrix(&temp[0], N);
     	
        if (power % 2 == 0) {
            temp = matrixBinPow(Adj_m, N, power / 2);
            calcMatMul(&temp[0], &temp[0], &result[0], N);
        }
        else {
            temp = matrixBinPow(Adj_m, N, power - 1);
            calcMatMul(&Adj_m[0], &temp[0], &result[0], N);                            
        }
		free(temp);  
	}
        
	return result;    
}
 
int main(int argc, const char* argv[]) {   
    size_t N = 4, power = 4;
    int iters = 5;
    size_t i, j, k;
    
    struct timeval start, end;
	double run_time = 0.0, average_runtime = 0.0;
    double r_time = 0.0;
	unsigned int tid, seed;

    if (argc > 1) {
    	N = atoi(argv[1]);
    	if (argc > 2) {
    		power = atoi(argv[2]);
    	}
    }
    
	omp_set_num_threads(nThreads);
	seedThreads();
	 
    double* A = (double *) malloc(N * N * sizeof(double));
    double* C = (double *) malloc(N * N * sizeof(double));
 
    randomMatrix(&A[0], N);
    
/*    
	printf("Initial matrix:\n");
	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%lf ", A[i * N + j]);
		}
		printf("\n");
	}   
*/

	for (int n = 0; n < iters; n++) {
		zeroMatrix(&C[0], N);
		
		gettimeofday(&start, NULL);    
		C = matrixBinPow(&A[0], N, power);	
		gettimeofday(&end, NULL);
		
		run_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
		average_runtime += run_time;
		printf("Running test #%d: time = %lf seconds\n", (n + 1), run_time);
	}
	
	average_runtime /= iters;
    printf("Average calculation time is %lf seconds\n", average_runtime);       

    free(A); 
    free(C);
    
    return 0;
}
