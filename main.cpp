#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
#include <math.h>

void *shearsort(void *arg);

void readMatrix();
void printMatrix();

void swap(int index, int col);
void swapCol(int row, int index);

int matrixSize = 4;
int matrix[4][4];

int complete = 0;

pthread_cond_t *cond;

pthread_mutex_t mutex;
pthread_t *threads;

int main()
{
	readMatrix();

	std::cout << "INPUT ARRAY:" << std::endl;
	std::cout << "------------" << std::endl;
	printMatrix();
	std::cout << std::endl;

	cond = (pthread_cond_t *)malloc(matrixSize * sizeof(pthread_cond_t));
	threads = (pthread_t *)malloc(matrixSize * sizeof(pthread_t));

	for (int x = 0; x < matrixSize; x++)
	{
		pthread_create(&threads[x], NULL, shearsort, &x);
		// shearsort(&x);
	}

	for (int i = 0; i < matrixSize; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *shearsort(void *arg)
{
	int index = *((int *)arg);

	for (int phase = 0; phase < log2(matrixSize * matrixSize) + 1; phase++)
	{
		if (phase % 2 == 0) // Alternating Bubble sort Phase
		{
			for (int i = 0; i < matrixSize - 1; i++)
			{
				for (int col = 0; col < matrixSize - i - 1; col++)
				{
					if (index % 2 == 0)
					{
						if (matrix[index][col] > matrix[index][col + 1])
						{
							swap(index, col);
							std::cout << "TEST1" << std::endl;
						}
					}
					else
					{
						if (matrix[index][col] < matrix[index][col + 1])
						{
							swap(index, col);
							std::cout << "TEST2" << std::endl;
						}
					}
				}
			}
		}
		else // Column sort Phase
		{
			std::cout << "TEST4" << std::endl;
			for (int i = 0; i < matrixSize - 1; i++)
			{
				for (int row = 0; row < matrixSize - i - 1; row++)
				{
					if (matrix[row][index] > matrix[row + 1][index])
					{
						std::cout << "TEST3" << std::endl;
						swapCol(row, index);
					}
				}
			}
		}

		// Wait for all threads to finish before starting next phase
		std::cout << "TEST6" << std::endl;
		pthread_mutex_lock(&mutex);
		complete++;
		if (complete != matrixSize)
		{
			pthread_cond_wait(&cond[index], &mutex);
		}
		else
		{
			std::cout << "Phase: " << phase + 1 << std::endl;
			std::cout << "--------" << std::endl;
			printMatrix();
			for (int i = 0; i < matrixSize; i++)
			{
				pthread_cond_signal(&cond[i]);
			}
			complete = 0;
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

void swap(int index, int col)
{
	int temp = matrix[index][col];
	matrix[index][col] = matrix[index][col + 1];
	matrix[index][col + 1] = temp;
}

void swapCol(int row, int index)
{
	int temp = matrix[row][index];
	matrix[row][index] = matrix[row + 1][index];
	matrix[row + 1][index] = temp;
}

void printMatrix()
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			std::cout << std::setw(2) << matrix[i][j];
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

void readMatrix()
{
	std::ifstream file("input.txt");

	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			file >> matrix[i][j];
		}
	}
}
