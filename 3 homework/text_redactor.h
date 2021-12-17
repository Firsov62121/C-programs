#include "string_builder.h"
enum type_of_cursor
{
    CENTER = 0,
    END = 1,
    PREEND = 2
};

typedef struct file
{
    string name;
    str_bld text;
    int curs_pos;
    str_node *curs_ptr;
    bool is_empty;
} file;

typedef struct window
{
    int height;
    int width;
    int curs_x;
    int curs_y;
    char **content;
    file w_file;
    int cursor_type; //enum
    bool **is_highlight;
    int row_prev;
    int stop_range;
} window;

file init_empty_file(void);
void clear_file(file *f);
window init_window(int height, int width, int cursor_type);
void open_file(const string name, window *w);
void change_file_name(file *f, const string s);
void save_file(file *fl);
void clear_window(window *w, bool need_save);
void clear_window_field(const window *w);
void print_window(const window *w);
int pos_in_line(str_node *ptr, str_node *first);
bool remove_highlighting(file *f);
void my_sync(window *w);
bool set_curs_type(window *w, int curs_type, int stop_range);
void add_string(str_bld *text, const string s, bool is_highlight);
void add_string_in_file(file *f, const string s, bool is_highlight);
bool set_cursor_pos(file *f, int new_pos);
void insert(file *f, const string s, int pos, bool is_highlight);
void put(file *f, const string s, bool is_highlight);
bool remove_char_range(file *f, int x, int y);
bool delete_left(file *f, int n);
bool move_curs_right(file *f, int n);
void uppercase_cursor(file *f);
void lowercase_cursor(file *f);
bool find(file *f, const string s);
bool replace(file *f, const string s, const string s2);