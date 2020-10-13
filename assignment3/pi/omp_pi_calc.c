#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int nThreads = 4;
const double EPS = 1e-09;
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

int main (int argc, char *argv[]) {
	double start, end;
	unsigned int i, tid, seed;
	unsigned int circle_points = 0;
	unsigned int total_points = 100000;
	
	if (argc > 1) {
		total_points = atoi(argv[1]);
	}

	omp_set_num_threads(nThreads);
	seedThreads();
	
	start = omp_get_wtime();
	#pragma omp parallel num_threads(nThreads) \
		private(i, tid, seed) \
		shared(total_points, seeds) \
		reduction(+: circle_points)
	{
		tid = omp_get_thread_num();
		seed = seeds[tid];
		srand(seed);
		
		#pragma omp for
		for (i = 0; i < total_points; i++) {
			double x = ((double) rand_r(&seed) / RAND_MAX);
			double y = ((double) rand_r(&seed) / RAND_MAX);
			
			if (x * x + y * y - 1 <= EPS) {
				circle_points++;
			}
		}
	}
	
	double Pi = 4. * circle_points / total_points;
	end = omp_get_wtime();
	
	printf("Pi = %lf\nCalculation time: %lf seconds\n", Pi, end - start);
	
	return 0;
}
