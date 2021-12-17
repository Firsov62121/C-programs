typedef struct str_node
{   
    struct str_node *next;
    struct str_node *prev;
    char value;
    bool is_highlighting;
} str_node;

typedef struct string
{
    char *str;
    int lenght;
} string;

typedef struct str_bld
{
    str_node *first;
    str_node *last;
    int lenght;
} str_bld;

typedef struct result_of_read_word
{
    string str;
    char end_with;
} result_of_read_word;

str_bld init_str_bld(void);
str_node * init_str_node(char c, bool is_highlighting);
void add_last_char(str_bld* s, char c, bool is_highlight);
void add_char_before(str_bld *sb, char c, str_node *before, bool is_highlight);
void delete_char(str_bld *sb, str_node *ptr);
void clear_str_bld(str_bld *s);
string init_string(const char *s);
string make_string(const str_bld s);
void clear_str(string *s);
result_of_read_word read_word(void);
string read_line(void);
bool are_equal_strings(const string s1, const string s2);
bool are_equal_string_char(const string s1, const char *s2);
string copy_str(const string s);