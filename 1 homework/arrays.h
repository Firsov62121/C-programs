typedef struct array
{
    int height;
    int *widths;
    int **arr;
} array;
void clear_array(array *const);
array initZeroMatrix(int);
void print_array(array, const char *);
array initTriangleArray(int);
array empty_array(void);
int * copy_array_string(const int * const, int);
void array_cpy(array *, const array, bool);
array copy_array(const array);
array copy_array_sort(const array);
void sort_array_string(int *, int);
int * copy_array_string_sort(const int * const, int);
bool are_equal_array_strings(const int * const, const int * const, int);
void sort_array(array a);
bool are_equal_arrays(array, array);