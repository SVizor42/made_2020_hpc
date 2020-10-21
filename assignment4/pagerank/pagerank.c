#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

double EPS = 0.0001;
int multiEdges = 1;
const int nThreads = 4;
unsigned int seeds[nThreads];
const char* fileName = NULL;

void seedThreads() {
	int my_thread_id;
	unsigned int seed;
	#pragma omp parallel private(seed, my_thread_id)
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

void zeroVector(double *A, const size_t N) {
	#pragma omp parallel for
	for (size_t i = 0; i < N; i++) {
		A[i] = 0.0;
	}
}
 
void randomMatrix(double *A, const size_t N) {
	srand(time(NULL));
	#pragma omp parallel for
	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++) {
			A[i * N + j] = rand() % (multiEdges + 1);
		}
}

void randomVector(double *x, const size_t N) {
	srand(time(NULL));
	#pragma omp parallel for
	for (size_t i = 0; i < N; i++) {
		x[i] = (double) rand() / RAND_MAX;
	}
}
 
double* calcMatVecMul(double *A, double *x, const size_t N) {
	size_t i, j, offset;
	
	double* result = (double *) malloc(N * sizeof(double)); 	
	zeroVector(&result[0], N);	

	#pragma omp parallel for private(i, j, offset)
 	for (i = 0; i < N; i++) {
 		offset = i * N;
		for (j = 0; j < N; j++) {
			result[i] += A[offset + j] * x[j];
		}
	}
	
    return result;
}

double calcNorm(double* x, const size_t N, const size_t type = 1) {
	double result = 0.0;
	size_t i;
	
/*	Calculate L1-norm	*/
	if (type == 1) {
		#pragma omp parallel for private(i) \
			reduction(+: result)
		for (i = 0; i < N; i++) {
			result += x[i];
		}
	}
/*	Else calculate L2-norm	*/
	else {
		#pragma omp parallel for private(i) \
			reduction(+: result)
		for (i = 0; i < N; i++) {
			result += x[i] * x[i];
		}
		result = sqrt(result);
	}

	return result;
}

void nomalizeVector(double* x, const size_t N, double norm) {
	size_t i;
	#pragma omp parallel for private(i) \
		shared(norm)
	for (i = 0; i < N; i++) {
		x[i] /= norm;
	}
}

double* pagerank(double *A, const size_t N, double damping = 0.85) {
	size_t i, j, offset;
	double norm, prev_norm;
	int iters = 0;

	double* result = (double *) malloc(N * sizeof(double));
//	randomVector(&result[0], N);

	#pragma omp parallel for private(i) \
		shared(N)		
	for (i = 0; i < N; i++) {
		result[i] = 1. / N;
	}
	
	norm = calcNorm(&result[0], N);
	nomalizeVector(&result[0], N, norm);
	
	double* A_hat = (double *) malloc(N * N * sizeof(double));
	zeroMatrix(&A_hat[0], N);

	#pragma omp parallel for private(i, j, offset) \
		shared(damping, N)
	for (i = 0; i < N; i++) {
		offset = i * N;
		for (j = 0; j < N; j++) {	
			A_hat[offset + j] = damping * A[offset + j] + (1 - damping) / N;
		}
	}
	
	prev_norm = 0.0;
	while (abs(norm - prev_norm) > EPS) {
		prev_norm = norm;
		
		result = calcMatVecMul(&A_hat[0], &result[0], N);
		norm = calcNorm(&result[0], N);
		nomalizeVector(&result[0], N, norm);
		
		iters++;
		printf("Iteration # %d: norm = %lf\n", iters, norm);
	}

	return result;
}

double* naiveRanking(double *A, const size_t N) {
	size_t i, j, offset;

	double* result = (double *) malloc(N * sizeof(double));
	zeroVector(&result[0], N);

	#pragma omp parallel for private(i, j, offset)  
	for (i = 0; i < N; i++) {
		offset = i * N;	
		for (j = 0; j < N; j++) {
			result[i] += A[offset + j];
		}
	}
		
	return result;
}
 
int main(int argc, const char* argv[]) {   
	size_t N = 5;
	int iters = 5;
	size_t i, j, k;

	struct timeval start, end;
	double run_time = 0.0;
	unsigned int tid, seed;

	if (argc > 1) {
		N = atoi(argv[1]);
		if (argc > 2) {
			EPS = atof(argv[2]);
			if (argc > 3) {
				fileName = argv[3];
			}    		
		}
	} 

	omp_set_num_threads(nThreads);
	seedThreads();

	char col_name[N][100];
	double* graph = (double *) malloc(N * N * sizeof(double));
	double* page_ranks = (double *) malloc(N * sizeof(double));
	double* naive_ranks = (double *) malloc(N * sizeof(double));

	memset(col_name, 0, sizeof(col_name[0][0]) * N * 100);
	
	#pragma omp parallel for private(i)  
	for (int i = 0; i < N; i++) {
		sprintf(col_name[i], "%d", i + 1);
	}

	zeroMatrix(&graph[0], N);

/*	Get data from file if available	*/    
	if (fileName != NULL) {   
	FILE* fp;
		fp = fopen(fileName, "r+");
		if (fp == NULL) {
			exit(EXIT_FAILURE);
		}
		
		char line[1024];
		char* ptr = NULL;
		i = 0;
		while (fgets(line, sizeof(line), fp)) {
			j = 0;
			ptr = strtok(line, ";");
			while (ptr != NULL)  {
				if (i > 0) {
					if (j == 0) {
						memcpy(&col_name[i - 1] , ptr, strlen(ptr));
					}
					else {
						graph[(i - 1) * N + (j - 1)] = (double) strtod(ptr, NULL);
					}
				}
				ptr = strtok(NULL, ";");
				j++;
			}
			i++;
		}
		
		fclose(fp);
	}
/*	Else generate data	*/
	else {
		randomMatrix(&graph[0], N);
	}

/*
	printf("Initial graph:\n");
	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%lf ", graph[i * N + j]);
		}
		printf("\n");
	}
*/

	gettimeofday(&start, NULL);  
	naive_ranks = naiveRanking(&graph[0], N);
	page_ranks = pagerank(&graph[0], N);
	gettimeofday(&end, NULL);
		
	run_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;

	printf("\nNaive ranking:\n");    
	for (i = 0; i < N; i++) {
		printf("%s: %lf\n", col_name[i], naive_ranks[i]);
	}

	printf("\nPagerank solution (percent):\n");    
	for (i = 0; i < N; i++) {
		printf("%s: %lf\n", col_name[i], page_ranks[i] * 100);
	}
	printf("\nRunning time = %lf seconds\n", run_time);	

	free(graph); 
	free(page_ranks);
 
	return 0;
}
