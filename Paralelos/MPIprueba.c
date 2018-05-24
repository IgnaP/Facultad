#include <mpi.h>
#include <stdio.h>

//mpicc -o HolaMundo HolaMundo.c
//mpirun -np 8 -machinefile Maquinas HolaMundo

int main(int argc, char *argv[]){
	int cantidad, identificador;
	int num;
	int resp;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &cantidad);
	MPI_Comm_rank (MPI_COMM_WORLD, &identificador);
	if (identificador==0) {
		palabra='Prueba';
		MPI_Send(num,1,MPI_INIT,1,1,MPI_COMM_WORLD);
		MPI_Recv(resp,1,MPI_INIT,1,2,MPI_COMM_WORLD);
		printf("%i",resp);
	}else{
		MPI_Recv(palabra,1,MPI_INIT,0,2,MPI_COMM_WORLD);
		if (num==10) {
			resp=0;
		}else{
			resp=1;
		}
		MPI_Send(resp,1,MPI_INIT,0,1,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
