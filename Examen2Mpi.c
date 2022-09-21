/*Utilizando la librería de MPI desarrollar un algoritmo 
que calcule la suma de un arreglo de números. 
El usuario ingresará el tamaño del arreglo y se llenara con números random, 
el arreglo de números debe ser dividido entre 
la cantidad total de procesos que se utilizarán de manera que cada proceso
 calcule una suma parcial. Al final el proceso 0 debe sumar todas 
 las sumas parciales e imprimir el resultado final de la suma.*/
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>
//array temporal para enviar
int a2[1000];
int multi[1000];

void process(int my_id, int processes, int a[], int size)
{
	MPI_Status status;

	int elementosRecibidos;
	if (my_id == 0)
	{
     for(;;){ 
        scanf("%d", &size);
        if (size > 0 && size <= 50)
            break;
        else
            printf("Invalid size, try again\n");
            
        }
        int a[size];
    for (int i = 0; i < size; i++)
    {
        if(i%2==0){
        a[i] = rand() % 100;
        
        printf(" %d ", a[i]);
        }else{
            multi[i]= rand() % 100;
            
        printf(" %d ", a[i]);
        }
    }
		int index, i;
		int balancear = size / processes;
		int elementosRecibidos;

		if (processes > 1)
		{
			for (i = 1; i < processes - 1; i++)
			{
				index = i * balancear;

				MPI_Send(&balancear,
					1, MPI_INT, i, 0,
					MPI_COMM_WORLD);
				MPI_Send(&a[index],
					balancear,
					MPI_INT, i, 0,
					MPI_COMM_WORLD);
			}

			index = i * balancear;
			int restantes = size - index;

			MPI_Send(&restantes,
				1, MPI_INT,
				i, 0,
				MPI_COMM_WORLD);
			MPI_Send(&a[index],
				restantes,
				MPI_INT, i, 0,
				MPI_COMM_WORLD);
		}

		float sum = 0.0;
        float mult=0.0;
		for (i = 0; i < balancear; i++)
			sum += a[i];

		int tmp;
		for (i = 1; i < processes; i++)
		{
			MPI_Recv(&tmp, 1, MPI_INT,
				MPI_ANY_SOURCE, 0,
				MPI_COMM_WORLD, &status);
			int sender = status.MPI_SOURCE;

			sum += tmp;
		}
        for (int i = 1; i < processes; i++)
        {
            MPI_Recv(&tmp, 1, MPI_INT,
				MPI_ANY_SOURCE, 0,
				MPI_COMM_WORLD, &status);
			int sender = status.MPI_SOURCE;

			mult += tmp;
        }
        

		printf("\nSize of array: %d\n", size);
        printf("Sum: %f\n", sum);
        printf("Mult: %f\n", mult);
        //promedio
        // float promedio = sum/(float)size;
        // printf("\nPromedio: %f\n", promedio);
	}
	else
	{
		MPI_Recv(&elementosRecibidos,
			1, MPI_INT, 0, 0,
			MPI_COMM_WORLD, &status);

		MPI_Recv(&a2, elementosRecibidos,
			MPI_INT, 0, 0,
			MPI_COMM_WORLD, &status);

		int partial_sum = 0;
        int partial_multi = 0;
        for (int i = 0; i < elementosRecibidos; i++)
			partial_multi *= multi[i];

		for (int i = 0; i < elementosRecibidos; i++)
			partial_sum += a2[i];

		MPI_Send(&partial_sum, 1, MPI_INT,
			0, 0, MPI_COMM_WORLD);

            MPI_Send(&partial_multi, 1, MPI_INT,
			0, 0, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[])
{
   int size;
   int a[50];
    
	int my_id, processes;

	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	process(my_id, processes, a, size);

	MPI_Finalize();
	return 0;

}