#include <stdio.h>
#include <string.h>
#include "my_string.h"

int main(void)
{
    string s1 = init_str("Hello");
    string s2 = init_empty_str(3, '7');
    string s3 = str_concat(s1, s2);
    string s4 = str_concat(&s1, &s3);
    print_str(s2, "s2");
    print_str(s1, "s1");
    print_str(s3, "s3");
    print_str(s4, "s4");

    change_str(&s4, "Hello, world!");
    change_str(&s1, substring(s4, 0, s4.length, 2));
    print_str(s4, "s4");
    print_str(s1, "s1");

    change_str(&s1, "1    23 3456 4gfd 5ref 6fref");

    str_arr arr1 = split(s1, ' ');
    print_str(s1, "s1");
    print_str_arr(arr1);
    clear_str_arr(&arr1);

    clear_str(&s1);
    clear_str(&s2);
    clear_str(&s3);
    clear_str(&s4);
    
    string s5 = init_str("abcd");
    string s6 = init_str("abcd");
    string s7 = init_str("1234");
    char *c8 = "abcd";
    char *c9 = "1234";
    char *c10 = "1234";
    print_str(s5, "s5");
    print_str(s6, "s6");
    print_str(s7, "s7");
    printf("c8 = \"%s\"\n", c8);
    printf("c9 = \"%s\"\n", c9);
    printf("c10 = \"%s\"\n", c10);
    printf("s5 ? s6 : %d\n", my_strcmp(s5, s6));
    printf("&s5 ? s7 : %d\n", my_strcmp(&s5, s7));
    printf("&s7 ? s6 : %d\n", my_strcmp(&s7, s6));
    printf("&s5 ? &s6 : %d\n", my_strcmp(&s5, &s6));
    printf("s5 ? c8 : %d\n", my_strcmp(s5, c8));
    printf("&s7 ? c9 : %d\n", my_strcmp(&s7, c9));
    printf("c9 ? c10 : %d\n", my_strcmp(c9, c10));
    printf("c8 ? s6 : %d\n", my_strcmp(c8, s6));
    printf("c9 ? &s7 : %d\n", my_strcmp(c9, &s7));

    change_str(&s5, "adadada");

    char * chr_ptr = my_strchr(s5, 'd');
    printf("VALUE OF chr_ptr : %c\n", *chr_ptr);
    char * chrr_ptr = my_strrchr(s5, 'd');
    printf("VALUE OF chrr_ptr : %c\n", *chrr_ptr);

    clear_str(&s5);
    clear_str(&s6);
    clear_str(&s7);
}