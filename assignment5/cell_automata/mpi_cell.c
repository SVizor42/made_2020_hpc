#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_line(int* line, const int size) {
    for (int i = 0; i < size; i++) {
        if (line[i] == 1) {
            printf("%s ", "■");
        }
        else {
            printf("%s ", "□");  
        }              
    }
    printf("\n");  
}

int implement_rule(int* cell, const int i, const int rule) {
    if (rule == 110) {
        if (cell[i - 1] == 0 && cell[i] == 0 && cell[i + 1] == 0 ||
            cell[i - 1] == 1 && cell[i] == 0 && cell[i + 1] == 0 ||
            cell[i - 1] == 1 && cell[i] == 1 && cell[i + 1] == 1) 
        {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        if (rule == 184) {
            if (cell[i - 1] == 0 && cell[i] == 0 && cell[i + 1] == 0 ||
                cell[i - 1] == 0 && cell[i] == 0 && cell[i + 1] == 1 ||
                cell[i - 1] == 0 && cell[i] == 1 && cell[i + 1] == 0 ||                   
                cell[i - 1] == 1 && cell[i] == 1 && cell[i + 1] == 0) 
            {
                return 0;
            }
            else {
                return 1;
            }
        }        
    }  
    
    return -1;
}

int main(int argc, char** argv) {
	int N;
    int batch_size = 4;
    int iters = 10;
    int periods = 1;
    int rule_id = 110;
	int tag = 42;
    int psize, prank;
    int ndim = 1, dims;
    MPI_Comm ring_1d;
    MPI_Status pstatus;
    int error;    

    MPI_Init(&argc, &argv);    
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

 	if (argc > 1) {
    	rule_id = atoi(argv[1]);
    }    
 	if (argc > 2) {
    	batch_size = atoi(argv[2]);
    }
 	if (argc > 3) {
    	periods = atoi(argv[3]);
    }   
 	if (argc > 4) {
    	iters = atoi(argv[4]);
    }
    
    dims = psize;
    MPI_Cart_create(MPI_COMM_WORLD, ndim, &dims, &periods, 0, &ring_1d);
    MPI_Comm_size(ring_1d, &psize);
    MPI_Comm_rank(ring_1d, &prank);
    
    N = batch_size * psize;
    int cell[batch_size + 2];

    srand(time(NULL) ^ prank);
    for (int i = 1; i < batch_size + 1; i++) {
        double value = (double) rand() / RAND_MAX;
        if (value > 0.5) {
            cell[i] = 1;
        } 
        else {
            cell[i] = 0;
        }
    }
    cell[0] = 0;
    cell[batch_size + 1] = 0;
    
    int line[N];
    int count[psize], disp[psize];
    
    for (int i = 0; i < psize; i++) {
        count[i] = batch_size;
        disp[i] = i * batch_size;  
    }
    
    MPI_Gatherv(&cell[1], batch_size, MPI_INT,
        &line[0], count, disp, MPI_INT, 0, ring_1d); 
    
    if (prank == 0) {
        print_line(&line[0], N);       
    }
    
    for (int j = 0; j < iters; j++) {
        int left, right;       
        MPI_Cart_shift(ring_1d, 0, 1, &left, &right);
        
        if (left >= 0) {
            MPI_Send(&cell[1], 1, MPI_INT, left, tag, ring_1d); 
        }
        if (right >= 0) {
            MPI_Send(&cell[batch_size], 1, MPI_INT, right, tag, ring_1d);       
        }        
       
        if (left >= 0) {
            MPI_Recv(&cell[0], 1, MPI_INT, left, tag, ring_1d, &pstatus);
        }        
        if (right >= 0) {        
            MPI_Recv(&cell[batch_size + 1], 1, MPI_INT, right, tag, ring_1d, &pstatus);              
        }  

        int buffer[batch_size + 2];
        if (!periods) {
            buffer[batch_size] = cell[batch_size];  
            buffer[1] = cell[1];
        }
        for (int i = 1 + (left < 0); i < batch_size + 1 - (right < 0); i++) {
            buffer[i] = implement_rule(&cell[0], i, rule_id);
        }

        memcpy(cell, buffer, (batch_size + 2) * sizeof(int));
        
        MPI_Gatherv(&cell[1], batch_size, MPI_INT,
            &line[0], count, disp, MPI_INT, 0, ring_1d);
            
        if (prank == 0) {
            print_line(&line[0], N); 
        }
    }
 	
    error = MPI_Finalize();

    return 0;
}
