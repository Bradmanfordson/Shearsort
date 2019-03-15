#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
using namespace std;

void bubble();
void sortdown();
void shearsort();
void readMatrix();
void printMatrix();

int matrixSize = 4;
int matrix[4][4];

int main()
{
	readMatrix();

	cout << "INPUT ARRAY:" << endl;
	cout << "------------" << endl;
	printMatrix();
	cout << endl;

	shearsort();

	return 0;
}

void shearsort()
{
	int x;
	for (x = 0; x < matrixSize + 1; x++)
	{
		if (x % 2 == 0)
		{
			bubble();
		}
		else
		{
			sortdown();
		}

		cout << "Phase " << x + 1 << ":" << endl;
		cout << "--------" << endl;
		printMatrix();
		cout << endl;
	}
}

void sortdown()
{
	int x, y, z;
	int count = 0;

	while (count < matrixSize - 1)
	{

		for (x = 0; x < matrixSize - 1; x++)
		{
			for (y = 0; y < matrixSize; y++)
			{
				if (matrix[x][y] > matrix[x + 1][y])
				{
					int temp = matrix[x][y];
					matrix[x][y] = matrix[x + 1][y];
					matrix[x + 1][y] = temp;
				}
			}
		}
		count++;
	}
}

void bubble()
{
	int x, y, z;
	int count = 0;

	while (count < matrixSize - 1)
	{

		for (x = 0; x < matrixSize; x++)
		{
			for (y = 0; y < matrixSize - 1; y++)
			{
				if (x % 2 == 0)
				{
					if (matrix[x][y] > matrix[x][y + 1])
					{
						int temp = matrix[x][y];
						matrix[x][y] = matrix[x][y + 1];
						matrix[x][y + 1] = temp;
					}
				}
				else
				{
					if (matrix[x][y] < matrix[x][y + 1])
					{
						int temp = matrix[x][y];
						matrix[x][y] = matrix[x][y + 1];
						matrix[x][y + 1] = temp;
					}
				}
			}
		}
		count++;
	}
}

void printMatrix()
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			cout << setw(2) << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void readMatrix()
{
	ifstream file("input.txt");

	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			file >> matrix[i][j];
		}
	}
}
