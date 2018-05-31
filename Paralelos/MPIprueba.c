#include <mpi.h>
#include <stdio.h>

//mpicc -o HolaMundo HolaMundo.c
//mpirun -np 8 -machinefile Maquinas HolaMundo

int main(int argc, char *argv[]){
	int cantidad, identificador;
	int num=999;
	int resp=999;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &cantidad);
	MPI_Comm_rank (MPI_COMM_WORLD, &identificador);
	MPI_Status estado;
	if (identificador==0) {
		num=10;
		MPI_Send(&num,1,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Recv(&resp,1,MPI_INT,1,1,MPI_COMM_WORLD,&estado);
		printf("%i\n",resp);
	}else{
		MPI_Recv(&num,1,MPI_INT,0,1,MPI_COMM_WORLD,&estado);
		if (num==10) {
			resp=0;
		}else{
			resp=1;
		}
		MPI_Send(&resp,1,MPI_INT,0,1,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
