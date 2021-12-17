#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "string_builder.h"

str_bld init_str_bld(void)
{
    str_bld s;
    s.lenght = 0;
    return s;
}

str_node * init_str_node(char c, bool is_highlighting)
{
    str_node *res = (str_node *)malloc(sizeof(str_node));
    res->is_highlighting = is_highlighting;
    res->value = c;
    return res;
} 

void add_last_char(str_bld* s, char c, bool is_highlight)
{
    if(s->lenght == 0)
    {
        s->first = init_str_node(c, is_highlight);
        s->last = s->first;
    }
    else if(s->lenght == 1)
    {
        s->last = init_str_node(c, is_highlight);
        s->last->prev = s->first;
        s->first->next = s->last;
    }
    else
    {
        str_node *ptr = s->last;
        s->last = init_str_node(c, is_highlight);
        s->last->prev = ptr;
        ptr->next = s->last;
    }
    ++s->lenght;
}

void add_char_before(str_bld *sb, char c, str_node *before, bool is_highlight)
{
    if(sb->lenght == 0)
    {
        add_last_char(sb, c, is_highlight);
    }
    else if(sb->first == before)
    {
        str_node *ptr = init_str_node(c, is_highlight);
        sb->first->prev = ptr;
        ptr->next = sb->first;
        sb->first = ptr;
        ++sb->lenght;
    }
    else
    {
        str_node *ptr = init_str_node(c, is_highlight);
        str_node *ptr2 = before->prev;
        ptr2->next = ptr;
        ptr->prev = ptr2;
        before->prev = ptr;
        ptr->next = before;
        ++sb->lenght;
    }
}

void delete_char(str_bld *sb, str_node *ptr)
{
    if(sb->lenght == 0)
        return;
    if(sb->lenght == 1)
    {
        sb->lenght = 0;
        free(sb->first);
        return;
    }
    if(sb->first == ptr)
    {
        ptr = ptr->next;
        free(sb->first);
        sb->first = ptr;
    }
    else if(sb->last == ptr)
    {
        ptr = ptr->prev;
        free(sb->last);
        sb->last = ptr;
    }
    else
    {
        str_node *next_ptr = ptr->next;
        str_node *prev_ptr = ptr->prev;
        free(ptr);
        next_ptr->prev = prev_ptr;
        prev_ptr->next = next_ptr;
    }
    --sb->lenght;
}

void clear_str_bld(str_bld *s)
{
    if(s->lenght == 0)
        return;
    str_node *ptr;
    for(int i = 0; i < s->lenght - 1; ++i)
    {
        ptr = s->first->next;
        free(s->first);
        s->first = ptr;
    }
    free(s->first);
    s->lenght = 0;
}

string init_string(const char *s)
{
    string s2;
    s2.lenght = strlen(s);
    s2.str = (char *)malloc(sizeof(char) * (s2.lenght + 1));
    memcpy(s2.str, s, sizeof(char) * (s2.lenght + 1));
    return s2;
}

string make_string(const str_bld s)
{
    string res;
    res.lenght = s.lenght;
    res.str = (char *)malloc(sizeof(char) * (s.lenght + 1));
    str_node *ptr = s.first;
    for(int i = 0; i < s.lenght - 1; ++i)
    {
        res.str[i] = ptr->value;
        ptr = ptr->next;
    }
    if(s.lenght != 0)
        res.str[s.lenght - 1] = ptr->value;
    res.str[s.lenght] = '\0';
    return res;
}

void clear_str(string *s)
{
    free(s->str);
    s->lenght = 0;
}

result_of_read_word read_word(void)
{
    char c = getchar();
    str_bld sb = init_str_bld();
    while(c != ' ' && c != '\n')
    {
        add_last_char(&sb, c, false);
        c = getchar();
    }
    
    result_of_read_word rrw;
    rrw.str = make_string(sb);
    clear_str_bld(&sb);
    rrw.end_with = c;
    return rrw;
}

string read_line(void)
{
    char c = getchar();
    str_bld sb = init_str_bld();
    while(c != '\n')
    {
        add_last_char(&sb, c, false);
        c = getchar();
    }
    string s = make_string(sb);
    clear_str_bld(&sb);
    return s;
}

bool are_equal_strings(const string s1, const string s2)
{
    if(s1.lenght != s2.lenght)
        return false;
    return !strcmp(s1.str, s2.str);
}

bool are_equal_string_char(const string s1, const char *s2)
{
    return !strcmp(s1.str, s2);
}

string copy_str(const string s) //const struct string s
{
    string s2;
    s2.lenght = s.lenght;
    s2.str = (char *)malloc((s.lenght + 1) * sizeof(char));
    memcpy(s2.str, s.str, (s.lenght + 1) * sizeof(char));
    return s2;
}