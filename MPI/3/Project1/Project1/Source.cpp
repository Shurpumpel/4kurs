#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
int main(int* argc, char** argv) {
	int ProcNum, ProcRank, RecvRank, SendRank;
	int numRing =5;
	MPI_Status Status;
	MPI_Request Request;
	char msg[10];
	char sendStr[10];
	int arrZero[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	int arr[2];

	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_Scatter(arrZero, 2, MPI_INT, arr, 2, MPI_INT, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
	{
		char message[] = "message";

		MPI_Send(message, _countof(message), MPI_CHAR, 1, 0, MPI_COMM_WORLD);

		printf("process rank %d send msg to 1 \n", ProcRank);
	}
	else {

		printf("process %d take vals: ", ProcRank);
		for (int i = 0; i < 2; i++) {
			printf(" %d", arr[i]);
		}
		printf("\n");

		for (int i = 0; i < numRing; i++) {
			
			if (i > 0 && ProcRank == 1) RecvRank = ProcNum - 1;
			else RecvRank = ProcRank - 1;
			

			MPI_Irecv(msg, _countof(msg), MPI_CHAR, RecvRank, 0, MPI_COMM_WORLD, &Request);

			if (ProcRank == ProcNum - 1) SendRank = 1;
			else SendRank = ProcRank + 1;

			int flag;

			MPI_Wait(&Request, &Status);

			printf("process num %d received msg :  \"%s\"  from process num %d for the %d time \n\n", ProcRank, msg, RecvRank, i);

			for (int k = 0; k < 10;  k++) {
				sendStr[k] = msg[k];
			}
			
			MPI_Send(sendStr, _countof(sendStr), MPI_CHAR, SendRank, 0, MPI_COMM_WORLD);
		}


	}
	MPI_Finalize();
	return 0;

}