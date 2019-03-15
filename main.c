#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *shearsort(void *arg);
void readMatrix();
void printMatrix();
void swap(int index, int col);
void swapCol(int row, int index);

int n = 4;
int matrix[4][4];

int complete = 0;

pthread_mutex_t mutex;
pthread_cond_t *cond;
pthread_t *threads;

int main()
{
    readMatrix();

    printf("Input Array: \n");
    printf("------------\n");
    printMatrix();
    printf("\n");

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

    return 0;
}

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
        // std::cout << "Test" << std::endl;
        pthread_mutex_lock(&mutex);
        complete++;
        if (complete != n)
        {
            pthread_cond_wait(&cond[index], &mutex);
        }
        else
        {
            printf("Phase: %d\n", phase + 1);
            printf("---------\n");
            printMatrix();
            printf("\n");
            // std::cout << "Index: " << index << std::endl;
            for (int i = 0; i < n; i++)
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
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void readMatrix()
{
    matrix[0][0] = 3;
    matrix[0][1] = 11;
    matrix[0][2] = 6;
    matrix[0][3] = 16;

    matrix[1][0] = 8;
    matrix[1][1] = 1;
    matrix[1][2] = 5;
    matrix[1][3] = 10;

    matrix[2][0] = 14;
    matrix[2][1] = 7;
    matrix[2][2] = 12;
    matrix[2][3] = 2;

    matrix[3][0] = 4;
    matrix[3][1] = 13;
    matrix[3][2] = 9;
    matrix[3][3] = 15;
}
