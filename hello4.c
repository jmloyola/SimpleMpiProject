#include <stdio.h>
#include <mpi.h>
#include <string.h>

main(int argc, char **argv){
	int numeroNodo;
	int numeroProcesos;
	char mensaje[30];
	int procesoEncargadoImpresion; // El parametro con el que se llama al programa indica el proceso encargado de la impresion. En caso de no encontrarse se toma al proceso 0 como encargado.
	int aux;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numeroNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);
		
	if (argc == 2){
		aux = atoi(argv[1]);
		if (aux < numeroProcesos){
			procesoEncargadoImpresion = aux;
		}
		else{
			procesoEncargadoImpresion = 0;
		}
	}
	else{
		procesoEncargadoImpresion = 0;
	}
		
	if (numeroNodo == procesoEncargadoImpresion){
		int procesoActual = 0;
		
		printf("Hola, soy el proceso %d (hay “%d” procesos) y recibo:\n",procesoEncargadoImpresion, numeroProcesos - 1);
		
		while (procesoActual < numeroProcesos){
			if (procesoActual != procesoEncargadoImpresion){
				MPI_Recv(mensaje, 30,MPI_CHAR, procesoActual, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				printf("\t %s\n",mensaje);
			}
			procesoActual++;
		}
	}
	else{
		sprintf(mensaje,"Hola desde el proceso %d", numeroNodo);
		MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR, procesoEncargadoImpresion, 1, MPI_COMM_WORLD); // No me interesa el tag que tenga el mensaje. No se como indicar ello.
																				// Notar que al resultado de strlen() le sumo uno ya que este no considera el \0 para calcular la longitud del string
	}
	
	MPI_Finalize();
}
