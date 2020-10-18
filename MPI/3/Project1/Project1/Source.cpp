#include <iostream>
#include <mpi.h>




using namespace std;

struct ComplexMatrix
{
	long long  Re;
	long long Im;
};
void  mult(struct ComplexMatrix* A, struct ComplexMatrix* B, ComplexMatrix* D, int N)//функция перемножающая две матрицы и возвращающая D
{
	ComplexMatrix sum;
	sum.Re = 0;
	sum.Im = 0;


	for (int i = 0; i < N; i++)
		for (int l = 0; l < N; l++)
		{
			sum.Re = 0;
			sum.Im = 0;
			for (int j = 0; j < N; j++)
			{
				sum.Im += A[i * N + j].Re * B[j * N + l].Im + B[j * N + l].Re * A[i * N + j].Im;
				sum.Re += A[i * N + j].Re * B[j * N + l].Re - A[i * N + j].Im * B[j * N + l].Im;
			}
			D[i * N + l].Re = sum.Re;
			D[i * N + l].Im = sum.Im;
		}


}

int main(int agrc, char* argv[])
{
	if (agrc != 3)
	{
		cerr << "Too few arguments\n";
		return 0;
	}
	ComplexMatrix Matrix;

	int N = atoi(argv[1]);
	int A = atoi(argv[2]);
	if (A == 1)
	{
		cerr << "Too few matrix\n";
		return 0;
	}
	int f = 0;
	bool t = 0;
	if (A % 2 == 0)
		f = A / 2;
	else
	{
		f = A / 2;
		t = 1;
	}
	ComplexMatrix** Matrix1 = new ComplexMatrix * [f + 1];
	ComplexMatrix** Matrix3 = new ComplexMatrix * [f + 1];
	for (int i = 0; i <= f; i++)
	{
		Matrix1[i] = new ComplexMatrix[N * N];
		Matrix3[i] = new ComplexMatrix[N * N];
	}
	ComplexMatrix* Matrix2 = new ComplexMatrix[N * N];
	ComplexMatrix* Matrix4 = new ComplexMatrix[N * N];
	ComplexMatrix* Matrix5 = new ComplexMatrix[N * N];
	ComplexMatrix* Matrix6 = new ComplexMatrix[N * N];

	int ProcNum, ProcRank;
	MPI_Datatype mystruct;//новый определяемый тип данных.
	double bigarray[2][2];
	MPI_Status Status;
	int blocklens[2];// количество элементов в каждов блоке,
	MPI_Aint  indices[2];//каждого блока от начала типа (в байтах)
	MPI_Datatype old_types[2];//oldtypes - исходные типы данных в каждом блоке в отдельности

	MPI_Init(&agrc, &argv);
	blocklens[0] = 1;
	blocklens[1] = 1;

	old_types[0] = MPI_INT;
	old_types[1] = MPI_INT;


	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	// The locations of each element 
	MPI_Address(&Matrix.Re, &indices[0]);
	MPI_Address(&Matrix.Im, &indices[1]);
	//Make relative */
	indices[1] = indices[1] - indices[0];
	indices[0] = 0;


	MPI_Type_struct(2, blocklens, indices, old_types, &mystruct);
	MPI_Type_commit(&mystruct);
	if (ProcNum - 1 >= f)
	{

		f++;
		if (ProcRank >= 0 && ProcRank < f)
		{
			Matrix2 = new ComplexMatrix[f * N * N];
			for (int i = 0; i < N * N; i++)
			{

				Matrix1[ProcRank][i].Re = 1 + rand() % 9;
				Matrix1[ProcRank][i].Im = 1 + rand() % 9;
				Matrix3[ProcRank][i].Re = 1 + rand() % 9;
				Matrix3[ProcRank][i].Im = 1 + rand() % 9;
				Matrix4[i].Re = 1 + rand() % 9;
				Matrix4[i].Im = 1 + rand() % 9;
				Matrix5[i].Im = 1 + rand() % 9;
				Matrix5[i].Im = 1 + rand() % 9;


			}




		}
		MPI_Group MPI_GROUP_WORLD;
		MPI_Group second;
		MPI_Comm comm;


		//Получить группу, относящуюся к MPI_COMM_WORLD 

		MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);

		int* process_ranks = new int[f];
		for (int i = 0; i < f; i++)
		{
			process_ranks[i] = i;
		}
		// Создать новую группу 
		MPI_Group_incl(MPI_GROUP_WORLD, f, process_ranks, &second);

		//Создать новый коммуникатор 
		MPI_Comm_create(MPI_COMM_WORLD, second, &comm);
		if (ProcRank >= 1 && ProcRank < f)
		{

			mult(Matrix1[ProcRank], Matrix1[ProcRank], Matrix3[ProcRank], N);



		}
		if (ProcRank >= 0 && ProcRank < f)
		{

			MPI_Gather(&Matrix3[ProcRank], N * N, mystruct, Matrix2, N * N, mystruct, 0, comm);
		}
		if (ProcRank == 0)
		{
			if (!t)
			{
				for (int i = 0; i < N * N; i++)
				{
					Matrix5[i].Re = Matrix2[2 * N * N + i].Re;
					Matrix5[i].Im = Matrix2[2 * N * N + i].Im;
				}

			}

			for (int i = !t + 1; i < f; i++)
			{
				for (int j = 0; j < N * N; j++)
				{
					Matrix4[j].Re = Matrix2[i * N * N + j].Re;
					Matrix4[j].Im = Matrix2[i * N * N + j].Im;

				}
				mult(Matrix5, Matrix4, Matrix6, N);
				for (int j = 0; j < N * N; j++)
				{
					Matrix5[j].Re = Matrix6[j].Re;
					Matrix5[j].Im = Matrix6[j].Im;
				}


			}
			for (int k = 0; k < N; k++)
			{
				cout << endl;
				for (int j = 0; j < N; j++)
				{

					cout << Matrix5[k * N + j].Re << " Proc" << k * N + j << " " << ProcRank << " " << Matrix5[k * N + j].Im << "             ";
				}
			}


		}

	}


	// <программный код с использованием MPI функций>
	MPI_Type_free(&mystruct);
	MPI_Finalize();



	return 0;
}
