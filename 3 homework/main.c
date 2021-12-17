#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include "text_redactor.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void beep(void)
{
    printf("\a");
}

void main_loop(int width, int height)
{
    window w = init_window(height, width, END);
    my_sync(&w);
    print_window(&w);
    while(true)
    {
        bool need_sync = false;
        result_of_read_word rrw = read_word();
        string s = rrw.str;
        if(are_equal_string_char(s, "exit"))
        {
            clear_str(&s);
            break;
        }
        else if(are_equal_string_char(s, "load"))
        {
            string name = read_line();
            open_file(name, &w);
            clear_str(&name);
            need_sync = true;
        }
        else if(are_equal_string_char(s, "add"))
        {
            remove_highlighting(&w.w_file);
            string s2 = read_line();
            add_string_in_file(&w.w_file, s2, false);
            need_sync = true;
            clear_str(&s2);
        }
        else if(are_equal_string_char(s, "move"))
        {
            string s2 = read_line();
            int x = atoi(s2.str);
            need_sync = set_cursor_pos(&w.w_file, x);
            clear_str(&s2);
        }
        else if(are_equal_string_char(s, "insert"))
        {
            remove_highlighting(&w.w_file);
            string s1 = read_word().str;
            int pos = atoi(s1.str);
            string s2 = read_line();
            if(pos < 0)
                pos = 0;
            insert(&w.w_file, s2, pos, false);
            clear_str(&s1);
            clear_str(&s2);
            need_sync = true;
        }
        else if(are_equal_string_char(s, "put"))
        {
            remove_highlighting(&w.w_file);
            string s2 = read_line();
            put(&w.w_file, s2, false);
            clear_str(&s2);
            need_sync = true;
        }
        else if(are_equal_string_char(s, "remove"))
        {
            need_sync = remove_highlighting(&w.w_file);
            string s1 = read_word().str;
            string s2 = read_word().str;
            int x = atoi(s1.str);
            int y = atoi(s2.str);
            if(remove_char_range(&w.w_file, x, y))
                need_sync = true;
            else
                beep();
            clear_str(&s1);
            clear_str(&s2);
        }
        else if(are_equal_string_char(s, "del"))
        {
            need_sync = remove_highlighting(&w.w_file);
            switch (rrw.end_with)
            {
            case '\n':
                delete_left(&w.w_file, 1);
                need_sync = true;
                break;
            case ' ':
                ;
                string s2 = read_line();
                int n = atoi(s2.str);
                clear_str(&s2);
                need_sync = delete_left(&w.w_file, n) || need_sync;
                break;
            }
        }
        else if(are_equal_string_char(s, "movelw"))
        {
            string s2 = read_line();
            int n = atoi(s2.str);
            clear_str(&s2);
            need_sync = move_curs_right(&w.w_file, -n);
        }
        else if(are_equal_string_char(s, "moverw"))
        {
            string s2 = read_line();
            int n = atoi(s2.str);
            clear_str(&s2);
            need_sync = move_curs_right(&w.w_file, n);
        }
        else if(are_equal_string_char(s, "upcase"))
        {
            remove_highlighting(&w.w_file);
            uppercase_cursor(&w.w_file);
            need_sync = true;
        }
        else if(are_equal_string_char(s, "lowcase"))
        {
            remove_highlighting(&w.w_file);
            lowercase_cursor(&w.w_file);
            need_sync = true;
        }
        else if(are_equal_string_char(s, "find"))
        {
            string s2 = read_line();
            bool tmp = remove_highlighting(&w.w_file);
            need_sync = find(&w.w_file, s2);
            if(!need_sync)
                beep();
            need_sync = tmp | need_sync;
            clear_str(&s2);
        }
        else if(are_equal_string_char(s, "replace"))
        {
            string s2 = read_word().str;
            string s3 = read_line();
            bool tmp = remove_highlighting(&w.w_file);
            need_sync = replace(&w.w_file, s2, s3);
            if(!need_sync)
                beep();
            need_sync = tmp | need_sync;
            clear_str(&s2);
            clear_str(&s3);
        }
        else if(are_equal_string_char(s, "save"))
        {
            switch (rrw.end_with)
            {
            case ' ':
                ;
                string s2 = read_line();
                change_file_name(&w.w_file, s2);
                clear_str(&s2);
                break;
            default:
                break;
            }
            save_file(&w.w_file);
        }
        else if(are_equal_string_char(s, "cursview"))
        {
            string s2 = read_word().str;
            if(are_equal_string_char(s2, "center"))
            {
                need_sync = set_curs_type(&w, CENTER, 0);
            }
            else if(are_equal_string_char(s2, "end"))
            {
                need_sync = set_curs_type(&w, END, 0);
            }
            else if(are_equal_string_char(s2, "preend"))
            {
                string s3 = read_word().str;
                int stop_line = atoi(s3.str);
                need_sync = set_curs_type(&w, PREEND, stop_line);
                clear_str(&s3);
            }
            clear_str(&s2);
        }
        if(need_sync)
        {
            clear_window_field(&w);
            my_sync(&w);
        } 
        print_window(&w);
        clear_str(&s);
    }
    clear_window(&w, false);
}

int main(void)
{
    main_loop(80, 25);
    return 0;
}