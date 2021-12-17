#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //string_bld.h + str_bld.c ; include guard - страж включения
#include "text_redactor.h"

static void start_highlight(void)
{
    printf("\033[1;33;45m");
}

static void start_border(void)
{
    printf("\033[0;40m");
}

static void end_special_text(void)
{
    printf("\033[0m");
}

static void print_cursor(char s)
{
    printf("\033[0;44m%c\033[0m", s);
}

static void print_highlight_cursor(char s)
{
    printf("\033[1;35;43m%c\033[0m", s);
}

static void print_left_right_border(void)
{
    start_border();
    printf("  ");
    end_special_text();
}

static void print_up_down_border(int width)
{
    start_border();
    for(int i = 0; i < width + 4; ++i)
        putchar(' ');
    end_special_text();
    putchar('\n');
}

file init_empty_file(void)
{
    file f;
    f.is_empty = true;
    f.text = init_str_bld();
    add_last_char(&f.text, ' ', false);
    f.curs_pos = 0;
    f.curs_ptr = f.text.first;
    return f;
}

void clear_file(file *f)
{
    clear_str_bld(&f->text);
    if(!f->is_empty)
        clear_str(&f->name);
    f->curs_pos = 0;
}

window init_window(int height, int width, int cursor_type)
{
    window w;
    w.curs_x = 0;
    w.curs_y = 0;
    w.cursor_type = cursor_type;
    w.height = height;
    w.width = width;
    w.content = (char **)malloc(sizeof(char *) * height);
    w.is_highlight = (bool **)malloc(sizeof(bool *) * height);
    for(int i = 0; i < height; ++i)
    {
        w.content[i] = (char *)malloc(sizeof(char) * width);
        w.is_highlight[i] = (bool *)malloc(sizeof(bool) * width);
        for(int j = 0; j < width; ++j)
        {
            w.content[i][j] = 0;
            w.is_highlight[i][j] = false;
        }
    }
    w.w_file = init_empty_file();
    w.row_prev = 0;
    w.stop_range = w.cursor_type == PREEND ? 1 : 0;
    return w;
}

void open_file(const string name, window *w)
{
    clear_file(&w->w_file);
    FILE *f = fopen(name.str, "r");
    if(f == NULL)
    {
        w->w_file = init_empty_file();
        return;
    }
    char c;
    bool has_char = false;
    while((c = fgetc(f)) != EOF)
    {
        if(!has_char)
        {
            has_char = true;
            w->w_file.is_empty = false;
            w->w_file.name = copy_str(name);
            w->w_file.curs_pos = 0;
            w->w_file.text = init_str_bld();
        }
        add_last_char(&w->w_file.text, c, false);
    }
    fclose(f);
    if(has_char)
        w->w_file.curs_ptr = w->w_file.text.first;
    else
        w->w_file = init_empty_file();
}

void change_file_name(file *f, const string s)
{
    if(!f->is_empty)
        clear_str(&f->name);

    f->name = copy_str(s);
}

void save_file(file *fl)
{
    FILE *f;
    if(fl->is_empty)
    {
        f = fopen("default.txt", "W");
        fclose(f);
        return;
    }
    f = fopen(fl->name.str, "w");
    str_node *ptr = fl->text.first;
    for(int i = 0; i < fl->text.lenght - 1; ++i)
    {
        fputc(ptr->value, f);
        ptr = ptr->next;
    }
    fputc(ptr->value, f);
    fclose(f);
}

void clear_window(window *w, bool need_save)
{
    w->curs_x = 0;
    w->curs_y = 0;
    clear_file(&w->w_file);
    for(int i = 0; i < w->height; ++i)
    {
        free(w->content[i]);
        free(w->is_highlight[i]);
    }
    free(w->content);
    free(w->is_highlight);
    w->cursor_type = CENTER;
    w->height = 0;
    w->width = 0;
}

void clear_window_field(const window *w)
{
    for(int i = 0; i < w->height; ++i)
    {
        for(int j = 0; j < w->width; ++j)
        {
            w->content[i][j] = 0;
        }
    }
}

void print_window(const window *w)
{
    system("clear");
    print_up_down_border(w->width);
    for(int i = 0; i < w->height; ++i)
    {
        print_left_right_border();
        int j = 0;
        while(j < w->width)
        {
            if(w->curs_x == i && w->curs_y == j){
                if(w->content[i][j] == 0)
                    print_cursor(' ');
                else if(w->is_highlight[i][j])
                    print_highlight_cursor(w->content[i][j]);
                else
                    print_cursor(w->content[i][j]);
                ++j;
            }
            else if(w->content[i][j] == 0)
            {
                while(j < w->width)
                {
                    if(i == w->curs_x && j == w->curs_y)
                    {
                        print_cursor(' ');
                        ++j;
                        continue;
                    }
                    putchar(' ');
                    ++j;
                }
                break;
            }
            else if(w->is_highlight[i][j])
            {
                start_highlight();
                while(j < w->width && w->is_highlight[i][j])
                {
                    if(w->curs_x == i && w->curs_y == j)
                    {
                        end_special_text();
                        print_highlight_cursor(w->content[i][j]);
                        start_highlight();
                    }
                    else
                        putchar(w->content[i][j]);
                    ++j;
                }
                end_special_text();
            }
            else
            {
                putchar(w->content[i][j]);
                ++j;
            }
        }
        print_left_right_border();
        putchar('\n');
    }
    print_up_down_border(w->width);
}

int pos_in_line(str_node *ptr, str_node *first)
{
    int k = 0;
    while(ptr != first && ptr->prev->value != '\n')
    {
        ++k;
        ptr = ptr->prev;
    }
    return k;
}

static int curs_row(const window *w)
{
    int k = 0;
    str_node *ptr = w->w_file.text.first;
    int j = 1;
    for(int i = 1; i <= w->w_file.curs_pos; ++i)
    {
        if((ptr->value == '\n' && ptr != w->w_file.curs_ptr) || j % w->width == 0)
        {
            ++k;
            if(ptr->value == '\n')
                j = 0;
        }
        ptr = ptr->next;
        ++j;
    }
    putchar('\n');
    return k;
}

bool remove_highlighting(file *f)
{
    bool res = false;
    str_node *ptr = f->text.first;
    for(int i = 0; i < f->text.lenght - 1; ++i)
    {
        if(ptr->is_highlighting)
        {
            res = true;
            ptr->is_highlighting = false;
        }
        ptr = ptr->next;
    }
    if(ptr->is_highlighting)
    {
        res = true;
        ptr->is_highlighting = false;
    }
    return res;
}

static void sync_by_curs_row(window *w, int curs_row)
{
    str_node *curs_ptr = w->w_file.curs_ptr;
    int k = pos_in_line(curs_ptr, w->w_file.text.first);
    w->curs_x = curs_row;
    w->curs_y = k % w->width;
    int x1 = w->curs_x, y1 = w->curs_y;
    str_node *ptr = curs_ptr;
    while(x1 < w->height)
    {
        if(ptr->value == '\n')
        {
            if(ptr == w->w_file.text.last)
                break;
            y1 = 0, ++x1;
            ptr = ptr->next;
            continue;
        }
        w->content[x1][y1] = ptr->value;
        w->is_highlight[x1][y1] = ptr->is_highlighting;
        ++y1;
        if(y1 >= w->width)
        {
            y1 = 0, ++x1;
        }
        if(ptr == w->w_file.text.last)
            break;
        ptr = ptr->next;
    }
    if(curs_ptr == w->w_file.text.first)
        return;
    x1 = w->curs_x, y1 = w->curs_y - 1;
    ptr = curs_ptr->prev;
    while(x1 >= 0)
    {
        if(y1 < 0 || ptr->value == '\n')
        {
            --x1;
            if(x1 < 0)
                return;
            y1 = pos_in_line(ptr, w->w_file.text.first) % w->width;
        }
        w->content[x1][y1] = ptr->value == '\n' ? ' ' : ptr->value;
        w->is_highlight[x1][y1] = ptr->is_highlighting;
        --y1;
        if(ptr == w->w_file.text.first)
            return;
        ptr = ptr->prev;
    }
}

void my_sync(window *w)
{
    clear_window_field(w);
    switch(w->cursor_type)
    {
        case CENTER:
            sync_by_curs_row(w, w->height / 2);
            break;
        case END:
        case PREEND:
            ;
            int curs_r = curs_row(w);
            int new_row = w->curs_x + curs_r - w->row_prev;
            if(new_row >= w->height - w->stop_range)
                new_row = w->height - 1 - w->stop_range;
            else if(new_row < w->stop_range)
                new_row = w->stop_range;
            sync_by_curs_row(w, new_row);
            w->row_prev = curs_r;
            break;
    }
}

bool set_curs_type(window *w, int curs_type, int stop_range)
{
    if(w->cursor_type == curs_type)
    {
        if(curs_type == PREEND)
        {
            if(w->stop_range == stop_range || stop_range <= 0 || stop_range >= w->width / 2)
                return false;
            w->stop_range = stop_range;
            return true;
        }
        return false;
    }
    if(curs_type == PREEND)
    {
        if(stop_range <= 0 || stop_range >= w->width / 2)
            return false;
        w->stop_range = stop_range;
    }
    else
        w->stop_range = 0;
    w->cursor_type = curs_type;
    return true;
}

void add_string(str_bld *text, const string s, bool is_highlight)
{
    for(int i = 0; i < s.lenght; ++i)
    {
        add_last_char(text, s.str[i], is_highlight);
    }
}

void add_string_in_file(file *f, const string s, bool is_highlight)
{
    if(f->is_empty)
    {
        if(s.lenght == 0)
            return;
        clear_str_bld(&f->text);
        f->text = init_str_bld();
        add_string(&f->text, s, is_highlight);
        f->is_empty = false;
        f->name = init_string("default.txt");
        f->curs_pos = 0;
        f->curs_ptr = f->text.first;
    }
    else
    {
        add_string(&f->text, s, is_highlight);
    }
}

static str_node * go_n_steps(str_node *ptr, int n, bool is_right)
{
    for(int i = 0; i < n; ++i)
    {
        if(is_right)
            ptr = ptr->next;
        else
            ptr = ptr->prev;
    }
    return ptr;
}

static str_node * find_ptr_by_pos(file *f, int pos)
{
    if(pos <= 0)
        return f->text.first;
    else if(pos >= f->text.lenght - 1)
        return f->text.last;
    else if(pos == f->curs_pos)
        return f->curs_ptr;
    else
    {
        if(f->curs_pos > pos)
        {
            if(f->curs_pos - pos > pos)
                return go_n_steps(f->text.first, pos, true);
            else
                return go_n_steps(f->curs_ptr, f->curs_pos - pos, false);
        }
        else
        {
            if(f->text.lenght - 1 - pos > pos - f->curs_pos)
                return go_n_steps(f->curs_ptr, pos - f->curs_pos, true);
            else
                return go_n_steps(f->text.last, f->text.lenght - 1 - pos, false);
        }
    }
}

bool set_cursor_pos(file *f, int new_pos)
{
    if(new_pos == f->curs_pos || new_pos >= f->text.lenght || new_pos < 0)
        return false;
    f->curs_ptr = find_ptr_by_pos(f, new_pos);
    f->curs_pos = new_pos;
    return true;
}

void insert(file *f, const string s, int pos, bool is_highlight)
{
    if(f->is_empty)
    {
        if(s.lenght == 0)
            return;
        clear_str_bld(&f->text);
        add_string(&f->text, s, is_highlight);
        f->is_empty = false;
        f->curs_ptr = f->text.first;
        f->name = init_string("default.txt");
        return;
    }
    if(pos >= f->text.lenght)
    {
        if(f->is_empty)
        add_string(&f->text, s, is_highlight);
        return;
    }
    str_node *ptr = find_ptr_by_pos(f, pos);
    int i;
    for(i = 0; i < s.lenght; ++i)
    {
        ptr->value = s.str[i];
        ptr = ptr->next;
        if(ptr == f->text.last)
        {
            if(i != s.lenght - 1)
            {
                ptr->value = s.str[i];
            }
            ++i;
            break;
        }
    }
    while(i < s.lenght)
    {
        ++i;
        add_last_char(&f->text, s.str[i], is_highlight);
    }
}

void put(file *f, const string s, bool is_highlight)
{
    if(f->is_empty)
    {
        if(s.lenght == 0)
            return;
        clear_str_bld(&f->text);
        add_string(&f->text, s, is_highlight);
        f->is_empty = false;
        f->curs_ptr = f->text.first;
        f->name = init_string("default.txt");
        return;
    }
    for(int i = 0; i < s.lenght; ++i)
        add_char_before(&f->text, s.str[i], f->curs_ptr, is_highlight);
    f->curs_pos += s.lenght;
}

bool remove_char_range(file *f, int x, int y)
{
    if(x > y || x < 0 || y >= f->text.lenght || f->is_empty == true)
        return false;
    str_node *ptr = find_ptr_by_pos(f, x);
    int delete_len = y - x + 1;
    if(f->text.lenght == delete_len)
    {
        clear_str_bld(&f->text);
        f->curs_pos = 0;
        add_last_char(&f->text, ' ', false);
        f->curs_ptr = f->text.first;
    }
    else if(y != f->text.lenght - 1)
    {
        for(int i = 0; i < delete_len; ++i)
        {
            str_node *ptr2 = ptr->next;
            delete_char(&f->text, ptr);
            ptr = ptr2;
        }
        if(f->curs_pos <= y && f->curs_pos >= x)
            f->curs_ptr = ptr;
        f->curs_pos = x;
    }
    else
    {
        for(int i = 0; i < delete_len; ++i)
        {
            str_node *ptr2 = ptr->prev;
            delete_char(&f->text, ptr);
            ptr = ptr2;
        }
        if(f->curs_pos >= x)
            f->curs_ptr = ptr;
        f->curs_pos = x - 1;
    }
    return true;
}

bool delete_left(file *f, int n)
{
    if(n <= 0 || n > f->curs_pos)
        return false;
    str_node *ptr = f->curs_ptr;
    f->curs_pos -= n;
    for(int i = 0; i < n; ++i)
    {
        delete_char(&f->text, ptr->prev);
    }
    return true;
}

bool move_curs_right(file *f, int n)
{
    if(f->curs_pos + n >= 0 && f->curs_pos + n < f->text.lenght)
    {
        f->curs_ptr = find_ptr_by_pos(f, f->curs_pos + n);
        f->curs_pos += n;
        return true;
    }
    return false;
}

void uppercase_cursor(file *f)
{
    f->curs_ptr->value = toupper(f->curs_ptr->value);
}

void lowercase_cursor(file *f)
{
    f->curs_ptr->value = tolower(f->curs_ptr->value);
}

bool find(file *f, const string s)
{
    if(s.lenght == 0)
        return false;
    bool res = false;
    str_node *ptr = f->text.first;
    int end_has = f->text.lenght;
    bool is_first = true;
    while(end_has >= s.lenght)
    {
        str_node *ptr2 = ptr;
        bool tmp_res = true;
        for(int i = 0; i < s.lenght; ++i)
        {
            if(ptr2->value != s.str[i])
            {
                tmp_res = false;
                break;
            }
            if(ptr2 != f->text.last)
                ptr2 = ptr2->next;
        }
        if(tmp_res)
        {
            if(is_first)
            {
                is_first = false;
                f->curs_ptr = ptr;
                f->curs_pos = f->text.lenght - end_has;
            }
            res = true;
            end_has -= s.lenght;
            for(int i = 0; i < s.lenght; ++i)
            {
                ptr->is_highlighting = true;
                if(ptr != f->text.last)
                    ptr = ptr->next;
            }
        }
        else
        {
            --end_has;
            if(end_has > 0)
                ptr = ptr->next;
        }
    }
    return res;
}

bool replace(file *f, const string s, const string s2)
{
    if(s.lenght == 0 || f->is_empty)
        return false;
    bool res = false;
    str_node *ptr = f->text.first;
    int end_has = f->text.lenght;
    bool is_first = true;
    while(end_has >= s.lenght)
    {
        str_node *ptr2 = ptr;
        bool tmp_res = true;
        for(int i = 0; i < s.lenght; ++i)
        {
            if(ptr2->value != s.str[i])
            {
                tmp_res = false;
                break;
            }
            if(ptr2 != f->text.last)
                ptr2 = ptr2->next;
        }
        if(tmp_res)
        {
            res = true;
            end_has -= s.lenght;
            for(int i = 0; i < s.lenght; ++i)
            {
                if(ptr != f->text.last)
                {
                    ptr2 = ptr->next;
                    delete_char(&f->text, ptr);
                    ptr = ptr2;
                }
                else if(ptr != f->text.first)
                {
                    ptr2 = ptr->prev;
                    delete_char(&f->text, ptr);
                    ptr = ptr2;
                    if(s2.lenght == 0)
                    {
                        if(is_first)
                        {
                            f->curs_pos = f->text.lenght - 1;
                            f->curs_ptr = ptr;
                        }
                        return true;
                    }
                }
                else
                {
                    clear_str_bld(&f->text);
                    f->curs_pos = 0;
                    if(s2.lenght == 0)
                    {
                        add_last_char(&f->text, ' ', true);
                        f->curs_ptr = f->text.first;
                    }
                    else
                    {
                        add_string_in_file(f, s2, true);
                        f->curs_ptr = f->text.first;
                    }
                    return true;
                }
            }
            if(s.lenght == 0)
            {
                if(is_first)
                {
                    is_first = false;
                    f->curs_pos = f->text.lenght - end_has;
                    f->curs_ptr = ptr;
                }
            }
            else
            {
                int tmp_pos = f->curs_pos;
                str_node *tmp_curs_ptr = f->curs_ptr;
                f->curs_pos = f->text.lenght - end_has;
                f->curs_ptr = ptr;
                put(f, s2, true);
                f->curs_ptr = tmp_curs_ptr;
                f->curs_pos = tmp_pos;
                if(is_first)
                {
                    is_first = false;
                    f->curs_pos = f->text.lenght - end_has - s.lenght;
                    f->curs_ptr = ptr;
                    for(int i = 0; i < s2.lenght; ++i)
                        f->curs_ptr = f->curs_ptr->prev;
                }
            }
        }
        else
        {
            --end_has;
            if(end_has > 0)
                ptr = ptr->next;
        }
    }
    return res;
}