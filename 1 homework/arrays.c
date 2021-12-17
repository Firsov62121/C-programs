#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arrays.h"

void clear_array(array *const a)
{
    for(int i = 0; i< a->height; ++i)
    {
        free(a->arr[i]);
    }
    if(a->height != 0)
    {
        free(a->widths);
        free(a->arr);
    }
    a->height = 0;
}

array initZeroMatrix(int n)
{
    array a;
    a.height = n;
    a.widths = (int *)malloc(n * sizeof(int));
    a.arr = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; ++i)
    {
        a.arr[i] = (int *)malloc(n * sizeof(int));
        for(int j = 0; j < n; ++j)
        {
            a.arr[i][j] = 0;
        }
        a.widths[i] = n;
    }
    return a;
}

void print_array(array a, const char *name)
{
    printf("array: %s\n", name);
    for(int i = 0; i < a.height; ++i)
    {
        for(int j = 0; j < a.widths[i]; ++j)
        {
            printf("%d ", a.arr[i][j]);
        }
        putchar('\n');
    }
}

array initTriangleArray(int n) 
{
    array a;
    a.height = n;
    a.widths = (int *)malloc(n * sizeof(int));
    a.arr = (int **)malloc(n * sizeof(int *));
    int k = 0;
    for(int i = 0; i < n; ++i)
    {
        a.arr[i] = (int *)malloc((n - i) * sizeof(int));
        for(int j = 0; j < n - i; ++j)
        {
            a.arr[i][j] = ++k;
        }
        a.widths[i] = n - i;
    }
    return a;
}

array empty_array(void) //return array with height = 0
{
    array a;
    a.height = 0;
    return a;
}

int * copy_array_string(const int * const str, int n)
{
    int * res = (int *)malloc(sizeof(int) * n);
    memcpy(res, str, sizeof(int) * n);
    return res;
}

void array_cpy(array * to, const array from, bool need_clear)
{
    if(need_clear)
        clear_array(to);
    to->height = from.height;
    if(from.height == 0)
        return;
    to->arr = (int **)malloc(sizeof(int *) * from.height);
    to->widths = copy_array_string(from.widths, from.height);
    for(int i = 0; i < from.height; ++i)
        to->arr[i] = copy_array_string(from.arr[i], from.widths[i]);
}

array copy_array(const array from)
{
    array a1;
    array_cpy(&a1, from, false);
    return a1;
}

array copy_array_sort(const array a)
{
    extern void sort_array_string(int *, int);
    array a2 = copy_array(a);
    for(int i = 0; i < a2.height; ++i)
    {
        sort_array_string(a2.arr[i], a2.widths[i]);
    }
    return a2;
}

static int min(int a, int b)
{
    return a > b ? b : a;
}

static void merge_sort_step(int *to, const int * const from, int i1, int i2, int n)
{
    int stop1 = min(i2, n);
    int stop2 = min(2 * i2 - i1, n);
    int cur = i1;
    while(i1 < stop1 && i2 < stop2)
    {
        if(from[i1] > from[i2])
            to[cur++] = from[i2++];
        else
            to[cur++] = from[i1++];
    }
    while(i1 < stop1)
        to[cur++] = from[i1++];
    while(i2 < stop2)
        to[cur++] = from[i2++];
}

void sort_array_string(int *str, int n) //merge sort
{
    int i = 1;
    int k = 0;
    int *str2 = (int *)malloc(sizeof(int) * n);
    while(i < n)
    {
        int m = i << 1;
        if(k % 2 == 0)
        {
            for(int j = 0; j < n; j += m)
                merge_sort_step(str2, str, j, j + i, n);
        }
        else
        {
            for(int j = 0; j < n; j += m)
                merge_sort_step(str, str2, j, j + i, n);
        }
        i <<= 1;
        ++k;
    }
    if(k % 2 != 0)
        memcpy(str, str2, n * sizeof(int));
    free(str2);
}

int * copy_array_string_sort(const int * const str, int n)
{
    int *str2 = copy_array_string(str, n);
    sort_array_string(str2, n);
    return str2;
}

bool are_equal_array_strings(const int * const str1, const int * const str2, int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(str1[i] != str2[i])
            return false;
    }
    return true;
}

static int num_of_digits(int n)
{
    int res = 0;
    while(n != 0)
    {
        ++res;
        n >>= 4;
    }
    return res;
}

static char * int_arr_to_str_arr(int *arr, int len)
{
    int k = 0;
    for(int i = 0; i < len; ++i)
        k += 1 + num_of_digits(abs(arr[i]));
    char *res = (char *)malloc(sizeof(char) * (k + 1));
    char *ptr = res;
    for(int i = 0; i < len; ++i)
    {
        *ptr = arr[i] > 0 ? '+' : '-';
        ++ptr;
        if(arr[i] == 0)
        {
            *ptr = '0';
            ++ptr;
            continue;
        }
        int n = arr[i];
        while(n != 0)
        {
            *ptr = '0' + n % 16;
            ++ptr;
            n >>= 4;
        }
    }
    *ptr = '\0';
    return res;
}

static int rows_cmp(int *row1, int *row2, int width1, int width2)
{
    if(width1 > width2)
        return -1;
    else if(width2 > width1)
        return 1;
    char *s1 = int_arr_to_str_arr(row1, width1);
    char *s2 = int_arr_to_str_arr(row2, width2);
    int res = strcmp(s1, s2);
    free(s1);
    free(s2);
    return res;
}

static void merge_sort_array_step(array to, const array from, int i1, int i2)
{
    int stop1 = min(i2, to.height);
    int stop2 = min(2 * i2 - i1, to.height);
    int cur = i1;
    while(i1 < stop1 && i2 < stop2)
    {
        if(rows_cmp(from.arr[i1], from.arr[i2], from.widths[i1], from.widths[i2]) > 0)
        {
            to.arr[cur] = from.arr[i2];
            to.widths[cur++] = from.widths[i2++];
        }
        else
        {
            to.arr[cur] = from.arr[i1];
            to.widths[cur++] = from.widths[i1++];
        }
    }
    while(i1 < stop1)
    {
        to.arr[cur] = from.arr[i1];
        to.widths[cur++] = from.widths[i1++];
    }
    while(i2 < stop2)
    {
        to.arr[cur] = from.arr[i2];
        to.widths[cur++] = from.widths[i2++];
    }
}

void sort_array(array a) //merge sort
{
    int i = 1;
    int k = 0;
    array a2;
    a2.height = a.height;
    a2.widths = (int *)malloc(sizeof(int) * a.height);
    memcpy(a2.widths, a.widths, sizeof(int) * a.height);
    a2.arr = (int **)malloc(sizeof(int *) * a.height);
    while(i < a.height)
    {
        int m = i << 1;
        if(k % 2 == 0)
        {
            for(int j = 0; j < a.height; j += m)
                merge_sort_array_step(a2, a, j, j + i);
        }
        else
        {
            for(int j = 0; j < a.height; j += m)
                merge_sort_array_step(a, a2, j, j + i);
        }
        i <<= 1;
        ++k;
    }
    if(k % 2 != 0)
    {
        memcpy(a.arr, a2.arr, a.height * sizeof(int *));
        memcpy(a.widths, a2.widths, a.height * sizeof(int));
    }
    free(a2.arr);
    free(a2.widths);
}

bool are_equal_arrays(array a1, array a2)
{
    if(a1.height != a2.height)
        return false;
    int n = a1.height;
    int *sort_a1_widths = copy_array_string_sort(a1.widths, n);
    int *sort_a2_widths = copy_array_string_sort(a2.widths, n);
    bool tmp = are_equal_array_strings(sort_a1_widths, sort_a2_widths, n);
    free(sort_a2_widths);
    free(sort_a1_widths);
    if(!tmp)
        return false;
    array a1_c = copy_array_sort(a1);
    array a2_c = copy_array_sort(a2);
    sort_array(a1_c);
    sort_array(a2_c);
    bool res = true;
    for(int i = 0; i < n; ++i)
    {
        if(!are_equal_array_strings(a1_c.arr[i], a2_c.arr[i], a1_c.widths[i]))
        {
            res = false;
            break;
        }
    }
    clear_array(&a1_c);
    clear_array(&a2_c);
    return res;
}