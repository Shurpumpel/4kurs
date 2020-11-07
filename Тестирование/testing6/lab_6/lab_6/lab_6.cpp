// lab_6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <thread>
#include <mutex>
#include <ctime>
#include <iostream>
#include <string>
#include <iostream>
using namespace std;

mutex mtx;
long** res = new long*[3];

long* mult(long* A, long* B, int M) {
	long* result = new long[M * M];
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			result[i * M + j] = 0;
			for (int k = 0; k < M; k++)
			{
				result[i * M + j] += A[i * M + k] * B[k * M + j];
			}
		}
	}

	return result;
}

void run(long** matrs, int M, int matrCount, int rank)
{
	long* result = new long[M * M];
	result = matrs[0];
	for (int i = 1; i < matrCount; i++) {
		result = mult(result, matrs[i], M);
	}
	mtx.lock();
	res[rank] = new long[M * M];
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			res[rank][i * M + j] = result[i * M + j];
		}
	}
	mtx.unlock();
}

int main()
{
	int M = 100;
	int matrCount = 3*5000;

	long** matrs = new long*[matrCount];

	for (int i = 0; i < matrCount; i++) {
		matrs[i] = new long[M * M];
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++)
			{
				matrs[i][j * M + k] = j + k;
				//cout << matrs[i][j * M + k] << "\t";
			}
			//cout << endl;
		}
		//cout << endl;
	}


	long** matrs1 = new long* [matrCount / 3];
	long** matrs2 = new long* [matrCount / 3];
	long** matrs3 = new long* [matrCount / 3];

	for (int i = 0; i < matrCount / 3; i++) {
		matrs1[i] = new long[M * M];
		matrs2[i] = new long[M * M];
		matrs3[i] = new long[M * M];

		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++) {
				matrs1[i][j * M + k] = matrs[i][j * M + k];
				matrs2[i][j * M + k] = matrs[i + matrCount / 3][j * M + k];
				matrs3[i][j * M + k] = matrs[i + 2 * matrCount / 3][j * M + k];
			}
		}
	}

	thread t1(run, matrs1, M, matrCount / 3, 0);
	thread t2(run, matrs2, M, matrCount / 3, 1);
	thread t3(run, matrs3, M, matrCount / 3, 2);

	t1.join();
	t2.join();
	t3.join();

	

	long* result = new long[M * M];
	result = res[0];
	for (int i = 1; i < 3; i++) {
		result = mult(result, res[i], M);
	}

	return 0;
}