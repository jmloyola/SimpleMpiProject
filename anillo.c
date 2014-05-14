#include <stdio.h>
#include <mpi.h>
#include <string.h>

main(int argc, char **argv){
	int numeroNodo;
	int numeroProcesos;
	char mensajeEntrada[30];
	char mensajeSalida[30];
	int cantidadVueltas;
	int aux;
	int vueltaActual = 0;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numeroNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);
		
	if (argc == 2){
		aux = atoi(argv[1]);
		cantidadVueltas = aux;
	}
	else{
		MPI_Finalize(); // No se si sea correcto
	}
	
	strcpy(mensajeSalida, "Mensaje");
		
	if (numeroNodo == 0){
		while (vueltaActual < cantidadVueltas){
			if (vueltaActual == 0){
				MPI_Send(mensajeSalida, strlen(mensajeSalida) + 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD); // No me interesa el tag que tenga el mensaje. No se como indicar ello.
				printf("El proceso %d envía un mensaje con el dato “A“ al proceso %d\n",numeroNodo, (numeroNodo + 1) % numeroProcesos);
			}
			else{
				MPI_Recv(mensajeEntrada, 30,MPI_CHAR, numeroProcesos - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(mensajeSalida, strlen(mensajeSalida) + 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
				printf("El proceso %d envía un mensaje con el dato “A“ al proceso %d\n",numeroNodo, (numeroNodo + 1) % numeroProcesos);
			}
			vueltaActual++;
		}
	}
	else{
		while (vueltaActual < cantidadVueltas){
			MPI_Recv(mensajeEntrada, 30,MPI_CHAR, numeroNodo - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(mensajeSalida, strlen(mensajeSalida) + 1, MPI_CHAR, (numeroNodo + 1) % numeroProcesos, 1, MPI_COMM_WORLD);
			printf("El proceso %d envía un mensaje con el dato “A“ al proceso %d\n",numeroNodo, (numeroNodo + 1) % numeroProcesos);
			
			vueltaActual++;
		}
	}
	
	MPI_Finalize();
}
