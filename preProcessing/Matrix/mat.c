#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Print the matrix
void Print_Mat(double* mat, int height, int width)
{
    for(int iHeight = 0; iHeight < width; iHeight++)
    {
        for(int iWidth = 0; iWidth < height; iWidth++)
        {
            printf(" %4f",mat[iHeight * width + iWidth]);
        }
        printf("\n");
    }
    printf("\n");
}

//Make the multiplication between two matrix
void Mult_Mat(double* matA, double* matB, int heightA, int widthA, int widthB, double* result)
{
    for(int iHeightA = 0; iHeightA < heightA ; iHeightA++)
    {
        for(int iWidthB = 0; iWidthB < widthB ; iWidthB++)
        {
            double sum = 0;
            for(int iWidthA = 0; iWidthA < widthA ; iWidthA++)
            {
                sum = sum + matA[iHeightA*widthA+iWidthA] * matB[iWidthA*widthB+iWidthB];
            }
            result[iHeightA*widthB+iWidthB] = sum;
        }   
    }
}

//Make the addition between two matrix
void Add_Mat(double* matA, double* matB, int height, int width)
{
    for(int iHeight = 0; iHeight < height ; iHeight++)
    {
        for(int iWidth = 0; iWidth < width ; iWidth++)
        {
             matA[iHeight * width + iWidth] = matA[iHeight * width + iWidth] + matB[iHeight * width + iWidth];
        }   
    }
}

//Make the addition between two matrix
void Sub_Mat(double* matA, double* matB, int height, int width)
{
    for(int iHeight = 0; iHeight < height ; iHeight++)
    {
        for(int iWidth = 0; iWidth < width ; iWidth++)
        {
             matA[iHeight * width + iWidth] = matA[iHeight * width + iWidth] - matB[iHeight * width + iWidth];
        }   
    }
}

//Make the transposition of a matrix
void Transpose_Mat(double* mat, double* result, int height, int width)
{
    for(int iHeight = 0; iHeight < height; iHeight++)
    {
        int i = iHeight/height;
        int j = iHeight%height;
        result[iHeight] = mat[width*j+i];
    }
}

//
void Trans_Mat(double* mat, double* temp, long size, long a, long b, int cols)
{
    int i = 0;
    int j = 0;

    //looping for each element of the martix
    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {
            if(row != a && col != b)
            {
                temp[i*cols+j] = mat[row*cols+col];

                if(j == size)
                {
                    j = 0;
                    i++;
                }
            }   
        }
    }
}


//Make the determinat of the matrix
double Det_Mat(double* mat, int size, int cols)
{
    double det = 0;

    if(size == 1)
        return mat[0];
    
    double* temp = (double*)calloc(cols* cols, sizeof(double));

    int sign = 1;

    for(int i = 0; i < size; i++)
    {
        Trans_Mat(mat, temp, size, 0, i, cols);
        det += sign * mat[i] * Det_Mat(temp, size-1, cols);

        sign = -sign;
    }

    free(temp);

    return det;
}


//
void Adj_Mat(double* mat, double* adj, int size)
{
    if(size == 0)
    {
        adj[0] = 1;
        return;
    }

    double sign = -1;
    double* temp = (double*)calloc(size*size, sizeof(double));

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            Trans_Mat(mat, temp, size, i, j , size);

            if((i+j)%2 == 0)
            {    
                sign = 1;
            }
            else
            {
                sign = -1;
            }

            adj[j*size+i] = sign * Det_Mat(temp, size-1, size);
        }
    }

    free(temp);
}

//Make the inverse of the matrix
int Inverse_Mat(double* mat, double* inverse, int size)
{
    double det = Det_Mat(mat, size, size);

    if(det == 0)
    {
        return 0;
    }

    double* adj = (double*)calloc(size*size, sizeof(double));
    Adj_Mat(mat, adj, size);

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            inverse[i*size+j] = adj[i*size+j]/det;
        }
    }

    free(adj);
    return 1;
}