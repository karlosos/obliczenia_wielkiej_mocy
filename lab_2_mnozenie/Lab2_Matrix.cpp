//============================================================================
// Name        : Lab2_Matrix.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>

#define N 10
using namespace std;

int matrix1[N][N];
int matrix2[N][N];

void fun(int a, int b)
{
	matrix2[a][b] = matrix1[b][a];
}

int main() {
	int a = 0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			matrix1[i][j] = a++;
		}
	}

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << matrix1[i][j] << " ";
		}
		cout << endl;
	}
	thread x[N][N];
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			x[i][j] = thread(fun, i, j);
		}
	}

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			x[i][j].join();
		}
	}

	cout << endl << endl << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << matrix2[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
