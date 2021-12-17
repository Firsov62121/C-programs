#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "arrays.h"

int ex_count;
int *offsets;

void set_ex_count(FILE *file)
{
    char c;
    int k = 0;
    long l;
    fpos_t *pos = (fpos_t *)&l;
    fgetpos(file, pos);
    ex_count = 0;
    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            ++k;
        else
            k = 0;
        if(k == 2)
        {
            k = 0;
            ++ex_count;
        }
    }
    fsetpos(file, pos);
}

int get_array_height(FILE *file)
{
    char c = 0;
    int k = 0;
    long l;
    fpos_t *pos = (fpos_t *)&l;
    fgetpos(file, pos);
    int height = -1;
    while(k < 2 && c != EOF)
    {
        c = fgetc(file);
        if(c == '\n')
        {
            ++height;
            ++k;
        }
        else
            k = 0;
    }
    fsetpos(file, pos);
    return height;
}

int get_array_width(FILE *file)
{
    char c = 0;
    long l;
    fpos_t *pos = (fpos_t *)&l;
    fgetpos(file, pos);
    int width = 1;
    while((c = fgetc(file))!= '\n')
    {
        if(c == ' ')
            ++width;
    }
    fsetpos(file, pos);
    return width;
}

int * get_array_row(FILE *file, int width)
{
    int *row = (int *)malloc(sizeof(int) * width);
    for(int i = 0; i < width; ++i)
        fscanf(file, "%d", &row[i]);
    fgetc(file);
    return row;
}

bool init_offsets(void)
{
    FILE *file = fopen("examples.txt", "r");
    if(file == NULL)
    {
        printf("Can't open file \"examples.txt\"\n");
        ex_count = 0;
        return false;
    }
    set_ex_count(file);
    offsets = (int *)malloc(sizeof(int) * ex_count);
    offsets[0] = 0;
    int i = 0;
    int k = 0;
    char c;
    int seek = 0;
    while((c = fgetc(file)) != EOF)
    {
        ++seek;
        if(c == '\n')
        {
            ++k;
            if(k == 2)
            {
                k = 0;
                offsets[++i] = seek;
                if(i == ex_count - 1)
                    break;
            }
        }
        else
            k = 0;
    }
    fclose(file);
    return true;
}

void clear_offsets(void)
{
    if(ex_count == 0)
        return;
    free(offsets);
    ex_count = 0;
}

array read_ex(int n)
{
    if(n >= ex_count || n < 0)
        return empty_array();        
    FILE* file = fopen("examples.txt", "r");
    if(file == NULL)
        return empty_array();
    fseek(file, offsets[n], SEEK_SET);
    array a;
    a.height = get_array_height(file);
    a.widths = (int *)malloc(sizeof(int) * a.height);
    a.arr = (int **)malloc(sizeof(int *) * a.height);
    for(int i = 0; i < a.height; ++i)
    {
        a.widths[i] = get_array_width(file);
        a.arr[i] = get_array_row(file, a.widths[i]);
    }
    fclose(file);
    return a;
}

void run_tests(void)
{
    FILE *file = fopen("tests.txt", "r");
    if(file == NULL)
    {
        fclose(file);
        return;
    }
    int test_num = 1;
    while(!feof(file))
    {
        int i, j, k;
        fscanf(file, "%d", &i);
        fscanf(file, "%d", &j);
        fscanf(file, "%d", &k);
        array a1 = read_ex(i);
        array a2 = read_ex(j);
        //print_array(a1, "a1");
        //print_array(a2, "a2");
        if(are_equal_arrays(a1, a2) == (bool)k)
            printf("test#%d passed\n", test_num);
        else
            printf("test#%d failed\n", test_num);
        ++test_num;
        clear_array(&a1);
        clear_array(&a2);
    }
    fclose(file);
}

int main()
{
    init_offsets();
    run_tests();
    clear_offsets();
}