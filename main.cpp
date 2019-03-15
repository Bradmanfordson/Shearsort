#include <pthread.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <mutex>

#include <sstream>
#include <vector>
#include <iterator>

/* Function Definitions */
void *shearsort(void *arg);
void swap(int index, int col);
void swapCol(int row, int index);
void printMatrix();
void readMatrix();
int rows();
int cols();
void sortRow(int index);
void sortRevRow(int index);
void sortColumns(int index);

/* Global Variables */
int n;
int complete = 0;
int **matrix;
int phases = 0;

pthread_mutex_t mutex;

int main()
{
	/* Main():
	1. Check that the input file has a square matrix, if not: exit, else: continue
	2. Read the matrix from the file
	3. Print the matrix
	4. Loop through the phases
		1. Create Threads
		2. print phase
		3. increment phase number
	5. Join Threads
	6. Exit
	*/
	if (rows() != cols()) // If matrix isn't square then exit program with error.
	{
		std::cout << "Matrix Dimention Error." << std::endl;
		return -1;
	}

	n = rows();

	matrix = new int *[n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new int[n];
	}
	readMatrix();

	std::cout << "Input Array:" << std::endl;
	std::cout << "------------" << std::endl;
	printMatrix();
	std::cout << std::endl;

	pthread_t *threads;
	threads = (pthread_t *)malloc(n * sizeof(pthread_t));

	while (phases < log2(n * n) + 1)
	{
		for (int x = 0; x < n; x++)
		{
			// std::cout << "x: " << x << std::endl;
			pthread_create(&threads[x], NULL, shearsort, &x);
		}

		for (int i = 0; i < n; i++)
		{
			pthread_join(threads[i], NULL);
		}

		phases++;
		std::cout << "Phase: " << phases << std::endl;
		std::cout << "--------" << std::endl;
		printMatrix();
		std::cout << std::endl;
	}

	pthread_mutex_destroy(&mutex);

	return 0;
}

void *shearsort(void *arg)
{
	// Enter critical section
	pthread_mutex_lock(&mutex);

	int index = *((int *)arg);
	index = index - 1;

	if (phases % 2 == 0) // Alternating Bubble sort Phase
	{
		if (index % 2 == 0)
		{
			sortRow(index);
		}
		else
		{
			sortRevRow(index);
		}
	}
	else // Column sort Phase
	{
		sortColumns(index);
	}

	// Exit critical section so unlock mutex and exit
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void sortRow(int index)
{
	for (int row = 0; row < n - 1; row++)
	{
		for (int col = 0; col < n - row - 1; col++)
		{
			if (matrix[index][col] > matrix[index][col + 1])
			{
				swap(index, col);
			}
		}
	}
}

void sortRevRow(int index)
{
	for (int row = 0; row < n - 1; row++)
	{
		for (int col = 0; col < n - row - 1; col++)
		{
			if (matrix[index][col] < matrix[index][col + 1])
			{
				swap(index, col);
			}
		}
	}
}

void sortColumns(int index)
{
	for (int col = 0; col < n - 1; col++)
	{
		for (int row = 0; row < n - 1; row++)
		{
			if (matrix[row][index] > matrix[row + 1][index])
			{
				swapCol(row, index);
			}
		}
	}
}

void swap(int index, int col)
{
	//	Take in the index value and column and swap two cells next to each other in the matrix
	int temp = matrix[index][col];
	matrix[index][col] = matrix[index][col + 1];
	matrix[index][col + 1] = temp;
}

void swapCol(int row, int index)
{
	//	Take in the index value and row and swap two cells above/below each other in the matrix
	int temp = matrix[row][index];
	matrix[row][index] = matrix[row + 1][index];
	matrix[row + 1][index] = temp;
}

void printMatrix()
{
	//	Itterate through each element in the matrix and print it to the terminal.
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

void readMatrix()
{
	//	Read integers from input.txt and assign them to appropriate cells in the matrix
	std::ifstream file("input.txt");

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			file >> matrix[i][j];
		}
	}
}

int rows()
{
	// Read the number of lines in the input file
	int lines = 0;
	std::string line;
	std::ifstream file("input.txt");

	while (std::getline(file, line))
	{
		lines++;
	}

	return lines;
}

int cols()
{
	// Found this algorithm online
	// Read the first line of the file
	// Counts the number of words in the line
	// Returns number of words in the line
	int words;
	std::string line;
	std::ifstream file("input.txt");

	std::getline(file, line);
	std::stringstream stream(line);
	words = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());

	return words;
}
