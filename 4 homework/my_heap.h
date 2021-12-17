#include <stdbool.h>
typedef int heap_type; //type of elems

typedef struct my_string
{
    char *str;
    int lenght;
} String;

typedef struct my_mem_block
{
    int lenght;
    heap_type *value;
} MemBlock;

typedef struct my_table_cell
{
    String id;
    MemBlock mem_block;
} TableCell;

typedef struct my_table_cell_list_node ListNode;

struct my_table_cell_list_node
{
    TableCell value;
    ListNode *next;
    ListNode *prev;
};

typedef struct my_table_cell_list
{
    int length;
    ListNode *first;
    ListNode *last;
} TableList;

typedef struct res_of_operation
{
    bool is_correct_operation;
    heap_type result;
    char *comment;
} OpRes;

OpRes init_arr(const char *id, int lenght, heap_type default_value);
void start_work(void);
void end_work(void);
void put(char *id, heap_type value);
OpRes putarr(char *id, int cell, heap_type value);
OpRes get(char *id);
OpRes getarr(char *id, int cell);
void print_opres(OpRes or);
void print_all_variables(void);