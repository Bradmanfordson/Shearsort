#include <iostream>
#include <fstream>
#include <array>
using namespace std;

void readMatrix();
void printMatrix();

int matrix[4][4];

int main()
{
	readMatrix();

	cout << "INPUT ARRAY:" << endl;
	cout << "------------" << endl;

	printMatrix();

	return 0;
}

void printMatrix()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout << matrix[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

void readMatrix()
{
	ifstream file("input.txt");

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			file >> matrix[i][j];
		}
	}
}
