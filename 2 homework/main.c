#include <stdio.h>
#include "matrix.h"
#include "point_vector.h"
#include <stdbool.h>

void printMatrixInFile(int** a, int h, int w, char *name)
{
    FILE *fp;
    if ((fp = fopen(name, "w")) == NULL)
    {
        printf("Не удалось открыть файл");
        getchar();
        return;
    }
    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            if(a[i * 2][j * 2])
                fprintf(fp, "%6d", a[i * 2][j * 2]);
            else
                fprintf(fp, "      ");
            if(j != w - 1)
            {
                char c = a[i * 2][j * 2 + 1] == -1 ? '-' : ' ';
                fprintf(fp, "%c", c);
            }
        }
        fprintf(fp, "\n");
        if(i != h - 1)
        {
            for(int j = 0; j < w; ++j)
            {
                char c = a[i * 2 + 1][j * 2] == -1 ? '|': ' ';
                fprintf(fp, "     %c ", c);
            }
            fprintf(fp, "\n");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
    getchar();
}

int devider_count(int n)
{
    int res = 0;
    for(int i = 2; i < n / 2 + 1; ++i)
    {
        if(n % i == 0)
            ++res;
    }
    return res;
}

enum is_horizontal {HORIZONTAL = true, VERTICAL = false};

void print_step(matrix *m, vector *v, bool dest)
{
    for(int i = 0; i < v->lenght; ++i)
    {
        vector v2 = init_vector(0);
        int row = v->arr[i].x;
        int col = v->arr[i].y;
        int value = m->arr[row][col];
        int k = 0;
        for(int j = 2; j < value / 2 + 1; ++j)
        {
            if(value % j == 0)
            {
                int row1 = (dest == HORIZONTAL) ? row : row + 2 + k;
                int col1 = (dest == HORIZONTAL) ? col + 2 + k : col;
                bool tmp = true;
                if(row1 < m->height && col1 < m->width)
                {
                    if(m->arr[row1][col1] != 0 && 
                        is_point_in_vector(v2, row1, col1) &&
                        devider_count(j) < devider_count(m->arr[row1][col1]))
                    {
                        tmp = false;
                    }
                }
                if(tmp)
                {
                    if(dest == HORIZONTAL)
                    {
                        add_matrix_elem(m, -1, row1, col1 - 1);
                        if(row1 > 0)
                            add_matrix_elem(m, 0, row1 - 1, col1);
                        if(col < m->width - 1)
                            add_matrix_elem(m, 0, row1, col1 + 1);
                        add_matrix_elem(m, 0, row1 + 1, col1);
                    }
                    else
                    {
                        add_matrix_elem(m, -1, row1 - 1, col1);
                        if(col1 > 0)
                            add_matrix_elem(m, 0, row1, col1 - 1);
                        if(row1 < m->height - 1)
                            add_matrix_elem(m, 0, row1 + 1, col1);
                        add_matrix_elem(m, 0, row1, col1 + 1);
                    }
                    add_matrix_elem(m, j, row1, col1);
                    add_point_if_absent(&v2, row1, col1);
                }
                k += 2;
            }
        }
        if(v2.lenght != 0)
        {
            print_step(m, &v2, !dest);
        }
        clear_vector(&v2);
    }
}

int main(void)
{
    int start, stop;
    scanf("%d %d", &start, &stop);
    if(start > stop)
    {
        int tmp = start;
        start = stop;
        stop = tmp;
    }
    if(start <= 0)
        start = 1;
    if(stop <= 0)
        stop = 1;
    matrix m = init_zero_matrix(1, (stop - start) * 2 + 1);
    vector v = init_vector(stop - start + 1);
    for(int i = start; i <= stop; ++i)
    {
        int tmp = (i - start) * 2;
        if(i != stop)
            add_matrix_elem(&m, -1, 0, tmp + 1);
        add_matrix_elem(&m, i, 0, tmp);
        add_point(&v, 0, tmp);
    }
    print_step(&m, &v, VERTICAL);
    suppres_zero_cols_rows(&m);
    printMatrixInFile(m.arr, m.height / 2 + m.height % 2, m.width / 2 + m.width % 2, "test.txt");
    clear_vector(&v);
    clear_matrix(&m);
}

/*
32 840
36 1260
40 1680
48 2520
60 5040
64 7560
72 10080
80 15120
84 20160
90 25200
96 27720 // норм
100 45360
108 50400
120 55440 // stop line
128 83160
144 110880
160 166320
168 221760
180 277200
192 332640
200 498960
216 554400
*/