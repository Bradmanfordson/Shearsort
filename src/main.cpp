// Author: Allen Bradley Roberts
// Date: March 15, 2019
// Course: Computer Science 352 - Operating Systems

// This file contains the entirety of Project 1.
// The entirety of multithreaded Shearsort is in this file,
// I used a mutex condition variable to handle synchronization.
// All needed functions have been added and defined in this file.
// All global variables needed have been defined in this file.

#include <pthread.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <mutex>
#include <sstream>
#include <vector>
#include <iterator>
#include <unistd.h>

/* Function Definitions */
void *shearsort(void *arg);
void sortRow(int index);
void sortRevRow(int index);
void sortColumns(int index);
void swap(int index, int col);
void swapCol(int row, int index);
void printMatrix();
void readMatrix();
int rows();
int cols();

/* Global Variables */
int n;
int complete = 0;
int **matrix;
int phases = 0;
pthread_mutex_t mutex;

int main()
{
	/* main():
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

	// Initialize matrix with given size n
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
		{ // create threads

			int *arg = (int *)malloc(sizeof(int));
			if (arg == NULL)
			{
				std::cout << "Couldn't allocate memory for thread arg." << std::endl;
				return -1;
			}
			*arg = x;
			pthread_create(&threads[x], 0, shearsort, arg);
		}

		for (int i = 0; i < n; i++)
		{ // join threads
			pthread_join(threads[i], NULL);
		}

		// increment phase and print matrix
		phases++;
		std::cout << "Phase: " << phases << std::endl;
		std::cout << "--------" << std::endl;
		printMatrix();
		std::cout << std::endl;
	}

	// clean up memory
	pthread_mutex_destroy(&mutex);

	for (int i = 0; i < n; i++)
	{
		delete matrix[i];
	}
	delete matrix;
	delete *matrix;
	delete threads;

	return 0;
}

void *shearsort(void *arg)
{
	// shearsort(): matrix sorting algorithm consisting of nothing more than
	// alternately sorting rows and columns of a matrix.
	// 1. lock mutex
	// 2. get indexnumber
	// 3. perform shearsort
	// 4. unlock mutex
	// 5. exit
	// Parameters: a void pointer argument which has the index number.

	// Enter critical section
	pthread_mutex_lock(&mutex);

	int index = *((int *)arg);
	delete (int *)arg;

	if (phases % 2 == 0) // Alternating Bubble sort Phase
	{
		if (index % 2 == 0) // Sort even rows
		{
			sortRow(index);
		}
		else // Reverse sort odd rows
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
	// sortRow: take in the index number and perform bubble sort on a specific
	// row of the matrix as indicated by index number.
	//Parameter: index integer to indicate matrix row number.

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
	// sortRevRow: take in the index number and perform a reverse bubble sort on a specific
	// row of the matrix as indicated by index number.
	//Parameter: index integer to indicate matrix row number.

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
	// sortColumns: take in the index number and perform basic bubble sort on a specific
	// column of the matrix as indicated by index number.
	//Parameter: index integer to indicate matrix column number.

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
	// swap: Take in the index value and column and swap two cells next to each other in the matrix
	//Parameter: index integer to indicate matrix row number.
	//Parameter: col integer to indicate column number.

	int temp = matrix[index][col];
	matrix[index][col] = matrix[index][col + 1];
	matrix[index][col + 1] = temp;
}

void swapCol(int row, int index)
{
	//swapCol: Take in the index value and row and swap two cells above/below each other in the matrix
	//Parameter: row integer to indicate row number.
	//Parameter: index integer to indicate matrix row number.

	int temp = matrix[row][index];
	matrix[row][index] = matrix[row + 1][index];
	matrix[row + 1][index] = temp;
}

void printMatrix()
{
	// printMatrix: Itterate through each element in the matrix and print it to the terminal.
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
	// readMatrix: Read integers from input.txt and assign them to appropriate cells in the matrix
	std::ifstream file("input.txt");

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			file >> matrix[i][j];
		}
	}
	file.close();
}

int rows()
{
	// rows: Read the number of lines in the input file and return the value
	// Returns line number as integer
	int lines = 0;
	std::string line;
	std::ifstream file("input.txt");

	while (std::getline(file, line))
	{
		lines++;
	}
	file.close();

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

	file.close();

	return words;
}
