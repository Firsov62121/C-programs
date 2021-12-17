#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "point_vector.h"

vector init_vector(int capacity)
{
    capacity = 1;
    vector v;
    v.lenght = 0;
    v.capacity = capacity;
    v.arr = (point *)malloc(capacity * sizeof(point));
    return v;
}

void clear_vector(vector *v)
{
    if(v->capacity != 0)
        free(v->arr);
    v->lenght = 0;
    v->capacity = 0;
}

void double_capacity(vector *v)
{
    if(v->capacity == 0)
    {
        v->arr = (point *)malloc(sizeof(point));
        v->capacity = 1;
        return;
    }
    point *arr = (point *)malloc(v->capacity * 2 * sizeof(point));
    memcpy(arr, v->arr, v->capacity * sizeof(point));
    free(v->arr);
    v->arr = arr;
    v->capacity <<= 1;
}

void add_point(vector *v, int x, int y)
{
    point p;
    p.x = x;
    p.y = y;
    if(v->capacity == v->lenght)
        double_capacity(v);
    v->arr[v->lenght] = p;
    ++v->lenght;
}

void add_point_if_absent(vector *v, int x, int y)
{
    for(int i = 0; i < v->lenght; ++i)
    {
        if(v->arr[i].x == x && v->arr[i].y == y)
            return;
    }
    add_point(v, x, y);
}

void print_vector(const vector v, const char *name)
{
    printf("vector: %s\n", name);
    for(int i = 0; i < v.lenght; ++i)
    {
        printf("(%d, %d) ", v.arr[i].x, v.arr[i].y);
    }
    putchar('\n');
}

bool is_point_in_vector(const vector v, int x, int y)
{
    for(int i = 0; i < v.lenght; ++i)
    {
        if(v.arr[i].x == x && v.arr[i].y == y)
            return true;
    }
    return false;
}