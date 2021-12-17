#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "matrix.h"

matrix init_zero_matrix(int height, int width)
{
    matrix m;
    m.height = height;
    m.width = width;
    m.arr = (int **)malloc(height * sizeof(int *));
    for(int i = 0;i < height; ++i)
    {
        m.arr[i] = (int *)malloc(sizeof(int) * width);
        for(int j = 0; j < width; ++j)
            m.arr[i][j] = 0;
    }
    return m;
}

void print_matrix(const matrix m, const char *name)
{
    printf("matrix: %s\n", name);
    for(int i = 0; i < m.height; ++i)
    {
        for(int j = 0; j < m.width; ++j)
        {
            printf("%4d ", m.arr[i][j]);
        }
        putchar('\n');
    }
}

void clear_matrix(matrix *m)
{
    for(int i = 0; i < m->height; ++i)
        free(m->arr[i]);
    free(m->arr);
    m->height = 0;
    m->width = 0;
}

void double_height(matrix *m)
{
    int **arr = (int **)malloc(m->height * 2 * sizeof(int *));
    for(int i = 0; i < m->height; ++i)
    {
        arr[i] = (int *)malloc(m->width * sizeof(int));
        memcpy(arr[i], m->arr[i], sizeof(int) * m->width);
    }
    for(int i = m->height; i < m->height * 2; ++i)
    {
        arr[i] = (int *)malloc(m->width * sizeof(int));
        //memset
        memset(arr[i], 0, m->width * sizeof(int));
    }
    int h = m->height, w = m->width;
    clear_matrix(m);
    m->width = w;
    m->height = h << 1;
    m->arr = arr;
}

void double_width(matrix *m)
{
    for(int i = 0; i < m->height; ++i)
    {
        int *arr = (int *)malloc(2 * m->width * sizeof(int));
        memcpy(arr, m->arr[i], sizeof(int) * m->width);
        //memset
        memset(arr + m->width, 0, m->width * sizeof(int));
        free(m->arr[i]);
        m->arr[i] = arr;
    }
    m->width <<= 1;
}

void add_matrix_elem(matrix *m, int value, int row, int col)
{
    while(row >= m->height)
        double_height(m);
    while(col >= m->width)
        double_width(m);
    m->arr[row][col] = value;
}

void suppres_zero_cols_rows(matrix *m)
{
    int k = 0;
    bool tmp = true;
    while(tmp)
    {
        for(int i = 0; i < m->width; ++i)
        {
            if(m->arr[m->height - k - 1][i] != 0)
            {
                tmp = false;
                break;
            }
        }
        ++k;
        if(k == m->height)
            break;
    }
    int new_height = m->height - k + 1;
    for(int i = new_height; i < m->height; ++i)
    {
        free(m->arr[i]);
    }
    int **arr = (int **)malloc(new_height * sizeof(int *));
    memcpy(arr, m->arr, new_height * sizeof(int *));
    free(m->arr);
    m->arr = arr;
    m->height = new_height;
    k = 0;
    tmp = true;
    while(tmp)
    {
        for(int i = 0; i < m->height; ++i)
        {
            if(m->arr[i][m->width - k - 1] != 0)
            {
                tmp = false;
                break;
            }
        }
        ++k;
    }
    int new_width = m->width - k + 1;
    if(new_width != m->width)
    {
        for(int i = 0; i < m->height; ++i)
        {
            int *arr2 = (int *)malloc(sizeof(int) * new_width);
            memcpy(arr2, m->arr[i], sizeof(int) * new_width);
            free(m->arr[i]);
            m->arr[i] = arr2;
        }
        m->width = new_width;
    }
}