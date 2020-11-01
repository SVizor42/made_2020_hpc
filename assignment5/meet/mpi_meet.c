#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct {
	int rank;
	char name[MPI_MAX_PROCESSOR_NAME];
} message;


int main(int argc, char** argv) {
	int N;
	int tag = 42;
    int psize, prank, len;
    int passed;
    MPI_Status pstatus;
    int error;    

    MPI_Init(&argc, &argv);    
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
 	
 	N = psize - 1;  
 	if (argc > 1) {
    	N = atoi(argv[1]);
    	if (N > psize - 1) {
    	    N = psize - 1;
    	}
    } 	
 	
    MPI_Datatype Message_type;
    int blocklen[2] = { 1, MPI_MAX_PROCESSOR_NAME }; 
	MPI_Aint disp[2] = { offsetof(message, rank), offsetof(message, name) };   
    MPI_Datatype type[2] = { MPI_INT, MPI_CHAR };
 	
 	MPI_Type_create_struct(2, blocklen, disp, type, &Message_type);
 	MPI_Type_commit(&Message_type);

	if (prank == 0) {

    	message init_msg = { prank, "" };
    	MPI_Get_processor_name(init_msg.name, &len);

 	    printf("Sending messages via %d processes.\n", psize);  	
        printf("Process rank #%d, name %s initilazes message exchange.\n", 
            init_msg.rank, init_msg.name);
    	
    	if (N > 0) {
	  		srand(time(NULL));
	  		int dest = rand() % (N - 1) + 1;
	   
	   		passed = 1;
			MPI_Ssend(&passed, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); 	
			MPI_Ssend(&init_msg, 1, Message_type, dest, tag, MPI_COMM_WORLD);
    	}
    }
    else {
    
		MPI_Recv(&passed, 1, MPI_INT, MPI_ANY_SOURCE, 
		        tag, MPI_COMM_WORLD, &pstatus);
				
		message recv_msg[passed + 1];
		
		MPI_Recv(&recv_msg, passed, Message_type, 
		        MPI_ANY_SOURCE, tag, 
		        MPI_COMM_WORLD, &pstatus);
		        
	  	recv_msg[passed].rank = prank;
    	MPI_Get_processor_name(recv_msg[passed].name, &len);	

		printf("\nStep %d of %d...\n", N - passed + 1, N);
		printf("Receiver: rank #%d, name %s\nSender: rank #%d, name %s\n", 
			recv_msg[passed].rank, recv_msg[passed].name, 
			recv_msg[passed - 1].rank, recv_msg[passed - 1].name);
			
		printf("Message content:\n");		
		for (int i = 0; i < passed; i++) {
			printf("#%d: name %s, rank #%d\n", 
			    i + 1, recv_msg[i].name, recv_msg[i].rank);
		}		
		
		if (passed < N) {
		
			int available[N - passed - 1];
			int idx = 0;
			for (int i = 0; i < psize; i++) {
				bool visited = false;
				for (int j = 0; j < (passed + 1); j++) {
					if (i == recv_msg[j].rank) {
						visited = true;
						break;
					}
				}
				if (!visited && prank != i) {
					available[idx] = i;
					idx++;
				}
			}
			
	  		srand(time(NULL));
	  		int dest = available[rand() % (psize - passed - 1)];
	  		 		
	  		passed++;
					
			MPI_Ssend(&passed, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); 	
			MPI_Ssend(&recv_msg, passed, Message_type, dest, tag, MPI_COMM_WORLD);
		}
	}
	
    MPI_Type_free(&Message_type);
    error = MPI_Finalize();

    return 0;
}
