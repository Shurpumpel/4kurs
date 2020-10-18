#include <omp.h>
#include <stdio.h>
#include <clocale>
#include <chrono>
int NMAX = 50900;
int LIMIT = 3000;
void main()
{
	setlocale(LC_ALL, "Russian");
	int i, j;
	float sum;


	//for (int k = 0; k < 10; k++) {
		//NMAX += 500;  
		float** a = new float* [NMAX];

		for (int i = 0; i < NMAX; i++)
		{
			a[i] = new float[NMAX];
		}

		for (i = 0; i < NMAX; i++)
			for (j = 0; j < NMAX; j++)
				a[i][j] = i + j;

		auto start_time = std::chrono::high_resolution_clock::now();
#pragma omp parallel shared(a) if (NMAX>=LIMIT)
		{
#pragma omp for private(i,j,sum) 
			for (i = 0; i < NMAX; i++)
			{
				sum = 0;
				for (j = 0; j < NMAX; j++)
					sum += a[i][j];
				//printf("Сумма элементов строки %d равна %f\n", i, sum);
			}
		} /* Завершение параллельного фрагмента */
		auto end_time = std::chrono::high_resolution_clock::now();;

		auto time = end_time - start_time;
		printf("NMAX = %d, LIMIT = %d Время выполнения: %d ms.\n", NMAX, LIMIT, time / std::chrono::milliseconds(1));
	//}
}


