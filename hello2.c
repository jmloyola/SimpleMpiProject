#include <stdio.h>
#include <mpi.h>
#include <string.h>

main(int argc, char **argv){
	int numeroNodo;
	int numeroProcesos;
	char mensaje[30];
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numeroNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);
	
	if (numeroNodo == 0){
		int cantidadMensajesRecibidos = 0;
		
		printf("Hola, soy el proceso 0 (hay “%d” procesos) y recibo:\n",numeroProcesos - 1);
		
		while (cantidadMensajesRecibidos < (numeroProcesos - 1)){
			MPI_Recv(mensaje, 30,MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("\t %s\n",mensaje);
			cantidadMensajesRecibidos++;
		}
	}
	else{
		sprintf(mensaje,"Hola desde el proceso %d", numeroNodo);
		MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD); // No me interesa el tag que tenga el mensaje. No se como indicar ello.
																				// Notar que al resultado de strlen() le sumo uno ya que este no considera el \0 para calcular la longitud del string
	}
	
	MPI_Finalize();
}
