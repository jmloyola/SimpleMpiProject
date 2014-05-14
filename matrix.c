#include <stdio.h>
#include <mpi.h>
#include <string.h>

main(int argc, char **argv){
	int numeroNodo;
	int numeroProcesos;
	int tamanioMatriz = 0;
	int i,j;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numeroNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);
	

	// Obtenemos el tamanio de la matriz desde los argumentos del programa.	
	if (argc == 2){
		tamanioMatriz = atoi(argv[1]);
	}
	else{
		printf("\nERROR: Debe pasar como parametro el tamaño de la matriz");
		MPI_Finalize(); // No se si sea correcto
		return;
	}


	int vector[tamanioMatriz]; //Lo coloco aca para luego hacer el broadcast.
	int matriz[tamanioMatriz][tamanioMatriz];
			
	if (numeroNodo == 0){
		int valoresParaMatriz = 1;

		//Genero los valores para la matriz.
		for (i=0; i<tamanioMatriz; i++){
			for (j=0; j<tamanioMatriz; j++){
				matriz[i][j] = valoresParaMatriz++; 
			}
		}

		valoresParaMatriz = 1;

		for (i=0; i<tamanioMatriz; i++){
			vector[i] = valoresParaMatriz++;
		}	
	}

	MPI_Bcast(vector,tamanioMatriz, MPI_INT, 0, MPI_COMM_WORLD);

	/*
	printf("\nSoy el proceso %d y los valores del vector por el que debo multiplicar son: ", numeroNodo);
	for (i=0; i<tamanioMatriz; i++){
		printf("\t%d",vector[i]);
	}
	printf("\n");*/

	int filaMatriz[tamanioMatriz];
	MPI_Scatter(matriz, tamanioMatriz, MPI_INT, filaMatriz, tamanioMatriz, MPI_INT, 0, MPI_COMM_WORLD);

	/*
	printf("\nSoy el proceso %d y los valores de la fila que debo multiplicar son: ", numeroNodo);
	for (i=0; i<tamanioMatriz; i++){
		printf("\t%d",filaMatriz[i]);
	}
	printf("\n");*/

	int resultadoMultiplicacionFila = 0;

	for (i=0; i<tamanioMatriz; i++){
		resultadoMultiplicacionFila += filaMatriz[i] * vector[i];
	}

	int vectorResultado[tamanioMatriz];	

	MPI_Gather(&resultadoMultiplicacionFila, 1, MPI_INT, vectorResultado, 1, MPI_INT, 0, MPI_COMM_WORLD); //Recordar que el primer parametro es la direccion al dato a ser enviado, por ello debe ser un parametro por referencia.

	if (numeroNodo == 0){
		printf("\nEl resultado de la multiplicacion es: ");
		for (i=0; i<tamanioMatriz; i++){
			printf("\n\t%d",vectorResultado[i]);
		}
		printf("\n");
	}
	
	MPI_Finalize();
}
