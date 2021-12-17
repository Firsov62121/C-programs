typedef struct string
{
    char *str;
    int length;
} string;

typedef struct string_array
{
    int length;
    string *strings;
} str_arr;

#define my_strcmp(A, B) _Generic((A),       \
    string: _Generic((B),                   \
        string: strings_strcmp,             \
        string *: string_string_ptr_strcmp, \
        char *: string_chars_strcmp         \
    ),                                      \
    string *: _Generic((B),                 \
        string: string_ptr_string_strcmp,   \
        string *: strings_ptr_strcmp,       \
        char *: string_ptr_chars_strcmp)    \
    ,                                       \
    char *: _Generic((B),                   \
        string: chars_string_strcmp,        \
        string *: chars_string_ptr_strcmp,  \
        char *: chars_chars_strcmp)         \
) (A, B)

#define str_concat(A, B)                    \
    _Generic((A),                           \
    string: string_concat,                  \
    string *: string_ptr_concat             \
)(A, B)

#define substring(A, B, C, D)               \
    _Generic((A),                           \
    string: string_substring,               \
    string *: string_ptr_substring          \
)(A, B, C, D)

#define split(A, B)                         \
    _Generic((A),                           \
    string: string_split,                   \
    string *: string_ptr_split              \
)(A, B)

#define change_str(A, B)                    \
    _Generic((B),                           \
    string: change_str_by_str,              \
    char *: change_str_by_chars             \
)(A, B)

int my_strlen(const char *s);
void my_strcpy(char *s1, const char *s2);
string init_str(const char *str);
void change_str_by_str(string *s, string new_s);
void change_str_by_chars(string *s, const char *new_s);
string init_empty_str(int len, char c);
string __init_empty_str(int len);
string string_ptr_concat(string *s1, string *s2);
string string_concat(string s1, string s2);
void clear_str(string *s);
void clear_str_arr(str_arr *arr);
void print_str(string s, const char *name);
void print_str_arr(str_arr arr);
string string_ptr_substring(string* s, int start, int stop, int step);
string string_substring(string s, int start, int stop, int step);
str_arr string_ptr_split(string* s, char c);
str_arr string_split(string s, char c);
char * my_strchr(string s, char c);
char * my_strrchr(string s, char c);
int strings_ptr_strcmp(string *s1, string *s2);
int string_ptr_chars_strcmp(string *s1, char *s2);
int strings_strcmp(string s1, string s2);
int string_chars_strcmp(string s1, char *s2);
int string_string_ptr_strcmp(string s1, string *s2);
int string_ptr_string_strcmp(string *s1, string s2);
int chars_string_strcmp(char *s1, string s2);
int chars_string_ptr_strcmp(char *s1, string *s2);
int chars_chars_strcmp(char *s1, char *s2);