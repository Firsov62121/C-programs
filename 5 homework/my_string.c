#include <stdlib.h>
#include <stdio.h>
#include "my_string.h"

int my_strlen(const char *s)
{
    int i;
    for(i = 0; s[i] != '\0'; ++i) ;
    return i;
}

void my_strcpy(char *s1, const char *s2)
{
    int i;
    for(i = 0; s2[i] != '\0'; ++i)
        s1[i] = s2[i];
    s1[i] = '\0';
}

string init_str(const char *str)
{
    string s;
    s.length = my_strlen(str);
    s.str = (char *)malloc((s.length + 1) * sizeof(char));
    my_strcpy(s.str, str);
    return s;
}

void change_str_by_str(string *s, string new_s)
{
    free(s->str);
    s->str = new_s.str;
    s->length = new_s.length;
}

void change_str_by_chars(string *s, const char *new_s)
{
    s->length = my_strlen(new_s);
    free(s->str);
    s->str = (char *)malloc((s->length + 1) * sizeof(char));
    my_strcpy(s->str, new_s);
}

string init_empty_str(int len, char c)
{
    extern string __init_empty_str(int);
    string s = __init_empty_str(len);
    for(int i = 0; i < len; i++)
    {
        s.str[i] = c;
    }
    return s;
}

string __init_empty_str(int len)
{
    string s;
    s.length = len;
    s.str = (char *)malloc((len + 1) * sizeof(char));
    s.str[len] = '\0';
    return s;
}


string string_ptr_concat(string *s1, string *s2)
{
    string s = __init_empty_str(s1->length + s2->length);
    for(int i = 0; i < s1->length; ++i)
    {
        s.str[i] = s1->str[i];
    }
    for(int i = 0; i < s2->length; ++i)
    {
        s.str[i + s1->length] = s2->str[i];
    }
    return s;
}

string string_concat(string s1, string s2)
{
    return string_ptr_concat(&s1, &s2);
}

void clear_str(string *s) // нет перегрузки для string, т.к. нужно корректировать длину
{
    free(s->str);
    s->length = 0;
}

void clear_str_arr(str_arr *arr)
{
    for(int i = 0; i < arr->length; ++i)
    {
        clear_str(&(arr->strings[i]));
    }
    free(arr->strings);
    arr->length = 0;
}

void print_str(string s, const char *name)
{
    printf("string %s: \"", name);
    for(int i = 0; i < s.length; ++i)
    {
        switch (s.str[i])
        {
        case '\n':
            printf("\\n");
            break;
        case '\t':
            printf("\\t");
            break;
        default:
            putchar(s.str[i]);
            break;
        }
    }
    printf("\";\n");
}

void print_str_arr(str_arr arr)
{
    printf("str_arr{\n");
    for(int i = 0; i < arr.length; ++i)
    {
        printf("    ");
        print_str(arr.strings[i], "arr_elem");
    }
    printf("}\n");
}

string string_ptr_substring(string* s, int start, int stop, int step)
{
    if(start < 0 || start >= stop || stop <= 0 || stop > s->length || step <= 0)
        return __init_empty_str(0);
    int tmp = stop - start;
    string res = __init_empty_str(tmp / step + (tmp % step == 0 ? 0 : 1));
    for(int i = start, j = 0; i < stop; i += step, ++j)
    {
        res.str[j] = s->str[i];
    }
    return res;
}

string string_substring(string s, int start, int stop, int step)
{
    return string_ptr_substring(&s, start, stop, step);
}

str_arr string_ptr_split(string* s, char c)
{
    int count = 0;
    for(int i = 0; i < s->length; ++i)
    {
        if(s->str[i] == c)
            count++;
    }
    str_arr res;
    count++;
    res.length = count;
    res.strings = (string *)malloc(count * sizeof(string));
    int k = 0, j = 0, i;
    for(i = 0; i < s->length; ++i)
    {
        if(s->str[i] == c)
        {
            res.strings[k] = substring(s, j, i, 1);
            ++k;
            j = i + 1;
        }
    }
    res.strings[k] = substring(s, j, i, 1);
    return res;
}

str_arr string_split(string s, char c)
{
    return string_ptr_split(&s, c);
}

char * my_strchr(string s, char c)
{
    for(int i = 0; i < s.length; ++i)
    {
        if(s.str[i] == c)
            return &s.str[i];
    }
    return NULL;
}

char * my_strrchr(string s, char c)
{
    for(int i = s.length; i >= 0;--i)
    {
        if(s.str[i] == c)
            return &s.str[i];
    }
    return NULL;
}
//change
int strings_ptr_strcmp(string *s1, string *s2)
{
    return chars_chars_strcmp(s1->str, s2->str);
}

int string_ptr_chars_strcmp(string *s1, char *s2)
{
    return chars_chars_strcmp(s1->str, s2);
}

int strings_strcmp(string s1, string s2)
{
    return chars_chars_strcmp(s1.str, s2.str);
}

int string_chars_strcmp(string s1, char *s2)
{
    return chars_chars_strcmp(s1.str, s2);
}

int string_string_ptr_strcmp(string s1, string *s2)
{
    return chars_chars_strcmp(s1.str, s2->str);
}

int string_ptr_string_strcmp(string *s1, string s2)
{
    return chars_chars_strcmp(s1->str, s2.str);
}

int chars_string_strcmp(char *s1, string s2)
{
    return chars_chars_strcmp(s1, s2.str);
}

int chars_string_ptr_strcmp(char *s1, string *s2)
{
    return chars_chars_strcmp(s1, s2->str);
}

//changed
int chars_chars_strcmp(char *s1, char *s2)
{
    int i = 0;
    for(i = 0; s1[i] != '\0' && s1[i] == s2[i]; ++i)
        ;
    if(s1[i] < s2[i])
        return -1;
    if(s1[i] > s2[i])
        return 1;
    return 0;
}