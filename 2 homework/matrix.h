typedef struct matrix
{
    int height;
    int width;
    int **arr;
} matrix;

matrix init_zero_matrix(int height, int width);
void print_matrix(const matrix m, const char *name);
void clear_matrix(matrix *m);
void double_height(matrix *m);
void double_width(matrix *m);
void add_matrix_elem(matrix *m, int value, int row, int col);
void suppres_zero_cols_rows(matrix *m);