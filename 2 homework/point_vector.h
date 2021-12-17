#include <stdbool.h>

typedef struct point
{
    int x;
    int y;
} point;

typedef struct vector
{
    int lenght;
    int capacity;
    point *arr;
} vector;

vector init_vector(int capacity);
void clear_vector(vector *v);
void double_capacity(vector *v);
void add_point(vector *v, int x, int y);
void add_point_if_absent(vector *v, int x, int y);
void print_vector(const vector v, const char *name);
bool is_point_in_vector(const vector v, int x, int y);