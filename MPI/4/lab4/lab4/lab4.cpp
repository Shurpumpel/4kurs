#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

//комплексное число
struct ComplexMatrix
{
    int Re;
    int Im;
};

ComplexMatrix* mult(struct ComplexMatrix* A, struct ComplexMatrix* B, int N)//функция перемножающая две матрицы и возвращающая D
{
    ComplexMatrix sum;
    ComplexMatrix* D = new ComplexMatrix[N * N];
    sum.Re = 0;
    sum.Im = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++) {
            sum.Re = 0;
            sum.Im = 0;
            for (int k = 0; k < N; k++)
            {
                sum.Re = sum.Re + A[i * N + k].Re * B[k * N + j].Re - A[i * N + k].Im * B[k * N + j].Im;
                sum.Im = sum.Im + A[i * N + k].Re * B[k * N + j].Im + A[i * N + k].Im * B[k * N + j].Re;
            }
            D[i * N + j].Re = sum.Re;
            D[i * N + j].Im = sum.Im;
        }
    }
    return D;
}

//вывод матрицы в консоль
void printMatrix(struct ComplexMatrix* res, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << res[i * N + j].Re << "+" << res[i * N + j].Im << "i\t";
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    int ProcNum, ProcRank;
	int N = 4;
	int A = 2;
    MPI_Status Status;
    ComplexMatrix matrix;
    bool flag = false;

    MPI_Init(&argc, &argv);

	MPI_Datatype complex_matr;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int blocklens[2];// количество элементов в каждов блоке,
    MPI_Aint  indices[2];//каждого блока от начала типа (в байтах)
    MPI_Datatype old_types[2];//oldtypes - исходные типы данных в каждом блоке в отдельности
    blocklens[0] = 1;
    blocklens[1] = 1;

    old_types[0] = MPI_INT;
    old_types[1] = MPI_INT;

    MPI_Address(&matrix.Re, &indices[0]);
    MPI_Address(&matrix.Im, &indices[1]);

    indices[1] = indices[1] - indices[0];
    indices[0] = 0;

    MPI_Type_struct(2, blocklens, indices, old_types, &complex_matr);
	MPI_Type_commit(&complex_matr);

    //если ранг != 0, то он принимает матрицу и домножает, если получил еще одну до тех пор, пока 0 ранг отправляет матрицы.
    if (ProcRank != 0)
    {
        ComplexMatrix* res = NULL;
        int recvcount = A / (ProcNum - 1);
        if (A % (ProcNum - 1) >= ProcRank)
            recvcount++;
        for (int i = 0; i < recvcount; i++)
        {
            ComplexMatrix* matr1 = new ComplexMatrix[N * N];
            MPI_Recv(matr1, N * N, complex_matr, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
            if (res == NULL)
            {
                //сохранение первой матрицы
                res = matr1;
            }
            else {
                //перемножение
                res = mult(res, matr1, N);
            }
        }
        if(recvcount>0)
            //отправка итоговой матрицы в 0 поток
            MPI_Send(res, N * N, complex_matr, 0, 0, MPI_COMM_WORLD);
    }
    else {
        int ss = 1;
        int sentcount = 0;
        ComplexMatrix* matr = new ComplexMatrix[N * N];
        
        //создание матрицы
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matr[i * N + j].Re = i+j;
                matr[i * N + j].Im = i+j;
            }
        }

        //рассылка матриц по потокам
        for (int i = 0; i < A; i++) {
            printMatrix(matr, N);
            MPI_Send(matr, N*N, complex_matr, ss,0, MPI_COMM_WORLD);
            ss = ss%(ProcNum-1)+1;
            sentcount++;
        }

        ComplexMatrix* matr2 = new ComplexMatrix[N * N];
        ComplexMatrix* res = NULL;

        int recvcount;
        if (sentcount < ProcNum-1)
            recvcount = sentcount + 1;
        else
            recvcount = ProcNum;

        //принятие матриц от потоков и перемножение их между собой
        for (int i = 1; i < recvcount; i++) {
            MPI_Recv(matr2, N * N, complex_matr, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
            if (res == NULL)
                res = matr2;
            else
                res = mult(res, matr2, N);
        }
        printMatrix(res, N);
    }

	MPI_Type_free(&complex_matr);
    MPI_Finalize();
    return 0;
}
