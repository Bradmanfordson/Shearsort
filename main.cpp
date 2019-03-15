#include <pthread.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

/* Function Definitions */
void *shearsort(void *arg);
void swap(int index, int col);
void swapCol(int row, int index);
void printMatrix();
void readMatrix();

/* Global Variables */
int n = 4;
int complete = 0;
int matrix[4][4];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *cond;

/* Main():
	1. Read the matrix from the file\
	2. Print the matrix
	3. Create Threads
	4. Join Threads
	5. Exit
*/
int main()
{
	pthread_t *threads;
	readMatrix();

	std::cout << "Input Array:" << std::endl;
	std::cout << "------------" << std::endl;
	printMatrix();
	std::cout << std::endl;

	cond = (pthread_cond_t *)malloc(n * sizeof(pthread_cond_t));
	threads = (pthread_t *)malloc(n * sizeof(pthread_t));

	for (int x = 0; x < n; x++)
	{
		pthread_create(&threads[x], NULL, shearsort, &x);
	}

	for (int i = 0; i < n; i++)
	{
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(cond);

	return 0;
}

// Shearsort():

void *shearsort(void *arg)
{
	int index = *((int *)arg);
	index = index - 1;

	for (int phase = 0; phase < log2(n * n) + 1; phase++)
	{
		if (phase % 2 == 0) // Alternating Bubble sort Phase
		{
			for (int i = 0; i < n - 1; i++)
			{
				for (int col = 0; col < n - i - 1; col++)
				{
					if (index % 2 == 0)
					{
						if (matrix[index][col] > matrix[index][col + 1])
						{
							swap(index, col);
						}
					}
					else
					{
						if (matrix[index][col] < matrix[index][col + 1])
						{
							swap(index, col);
						}
					}
				}
			}
		}
		else // Column sort Phase
		{
			for (int i = 0; i < n - 1; i++)
			{
				for (int row = 0; row < n - i - 1; row++)
				{
					if (matrix[row][index] > matrix[row + 1][index])
					{
						swapCol(row, index);
					}
				}
			}
		}

		// Wait for all threads to finish before starting next phase
		pthread_mutex_lock(&mutex);
		complete++;

		if (complete == n) // When all threads have completed, print matrix, signal conditions, and set complete to 0
		{
			std::cout << "Phase: " << phase + 1 << std::endl;
			std::cout << "--------" << std::endl;
			printMatrix();
			std::cout << std::endl;

			for (int i = 0; i < n; i++)
			{
				pthread_cond_signal(&cond[i]);
			}

			complete = 0;
		}
		else // While all threads aren't complete, wait on the condition
		{
			pthread_cond_wait(&cond[index], &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

// Swap():
//	Take in the index value and column and swap two cells next to each other in the matrix
void swap(int index, int col)
{
	int temp = matrix[index][col];
	matrix[index][col] = matrix[index][col + 1];
	matrix[index][col + 1] = temp;
}

// SwapCol():
//	Take in the index value and row and swap two cells above/below each other in the matrix
void swapCol(int row, int index)
{
	int temp = matrix[row][index];
	matrix[row][index] = matrix[row + 1][index];
	matrix[row + 1][index] = temp;
}

// printMatrix()
//	Itterate through each element in the matrix and print it to the terminal.
void printMatrix()
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			std::cout << std::setw(2) << matrix[i][j];
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

// readmatrix()
//	Read integers from input.txt and assign them to appropriate cells in the matrix
void readMatrix()
{
	std::ifstream file("input.txt");

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			file >> matrix[i][j];
		}
	}
}
