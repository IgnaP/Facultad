#include <mpi.h>
#include <stdio.h>

//mpicc -o HolaMundo HolaMundo.c
//mpirun -np 8 -machinefile Maquinas HolaMundo

int main(int argc, char *argv[]){
	int cantidad, identificador;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &cantidad);
	MPI_Comm_rank (MPI_COMM_WORLD, &identificador);
	printf("Hola Mundo Soy %d de %d \n",identificador, cantidad);
	MPI_Finalize();
}
