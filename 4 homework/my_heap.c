#include "my_heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CORRECT_OPERATION "Корректная операция"
#define INCORRECT_LENGHT_OF_ARRAY "Неверная длина массива"
#define INDEX_OUT_OF_RANGE_EXCEPTION "Выход за границы массива"
#define UNINICIALIZED_VALUE "Обращение к неинициализорованной переменной"
#define UNINICIALIZED_ARRAY "Обращение к неинициализорованному массиву"
#define EMPTY_STR ""
#define TABLE_SIZE 256

static heap_type default_heap_type = 0;

static bool is_init = false;
static TableList *table;

static void clear_str(String *s)
{
    if(s->lenght != 0)
        free(s->str);
    s->lenght = 0;
}

static void clear_memblock(MemBlock *mb)
{
    if(mb->lenght != 0)
        free(mb->value);
    mb->lenght = 0;
}

static void clear_tablecell(TableCell *tc)
{
    clear_str(&tc->id);
    clear_memblock(&tc->mem_block);
}

static void clear_listnode(ListNode *ln)
{
    clear_tablecell(&ln->value);
    free(ln);
}

static void clear_list(TableList *l)
{
    if(l->length == 0)
        return;
    ListNode *ptr = l->first;
    for(int i = 0; i < l->length - 1; ++i)
    {
        ListNode *ptr2 = ptr->next;
        clear_listnode(ptr);
        ptr = ptr2;
    }
    clear_listnode(ptr);
    l->length = 0;
}

static TableList empty_list(void)
{
    TableList res;
    res.length = 0;
    return res;
}

void start_work(void)
{
    if(is_init)
        return;
    table = (TableList *)malloc(sizeof(TableList) * TABLE_SIZE);
    for(int i = 0; i < TABLE_SIZE; ++i)
        table[i] = empty_list();
    is_init = true;
}

void end_work(void)
{
    if(!is_init)
        return;
    for(int i = 0; i < TABLE_SIZE; ++i)
        clear_list(&table[i]);
    free(table);
    is_init = false;
}

static int hash_from_str(const char *s)
{
    int res = 0;
    for(int i = 0; s[i] != '\0'; ++i)
        res += (int)s[i];
    return res % TABLE_SIZE;
}

static ListNode * is_in_list(const TableList *list, const char *id)
{
    if(list->length == 0)
        return NULL;
    ListNode *ptr = list->first;
    for(int i = 0; i < list->length; ++i, ptr = ptr->next)
    {
        if(!strcmp(id, ptr->value.id.str))
            return ptr;
    }
    return NULL;
}

static String get_string_from_chars(const char *s)
{
    String res;
    res.lenght = strlen(s);
    res.str = (char *)malloc((res.lenght + 1) * sizeof(char));
    memcpy(res.str, s, (res.lenght + 1) * sizeof(char));
    return res;
}

static void add_last_list_elem(TableList *l, ListNode *ptr)
{
    if(l->length == 0)
    {
        l->first = l->last = ptr;
    }
    else if(l->length == 1)
    {
        l->last = ptr;
        l->last->prev = l->first;
        l->first->next = l->last;
    }
    else
    {
        ptr->prev = l->last;
        l->last->next = ptr;
        l->last = ptr;
    }
    ++l->length;
}

static void add_id(TableList *l, const char *id, const MemBlock mb)
{
    ListNode *ptr = (ListNode *)malloc(sizeof(ListNode));
    ptr->value.id = get_string_from_chars(id);
    ptr->value.mem_block = mb;
    add_last_list_elem(l, ptr);
}

static MemBlock get_array(int lenght, heap_type default_value)
{
    MemBlock mb;
    mb.lenght = lenght;
    mb.value = (heap_type *)malloc(sizeof(heap_type) * lenght);
    for(int i = 0; i < lenght; ++i)
        mb.value[i] = default_value;
    return mb;
}

static void delete_node_from_list(TableList *l, ListNode *ptr)
{
    if(l->length == 0)
        return;
    if(l->length == 1)
        clear_listnode(ptr);
    else if(l->first == ptr || l->last == ptr)
    {
        clear_listnode(ptr);
    }
    else
    {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        clear_listnode(ptr);
    }
    --l->length;
}

static void add_id_correctly(const char *id, const MemBlock mb)
{
    TableList *l = &table[hash_from_str(id)];
    ListNode *tmp_ptr = is_in_list(l, id);
    if(tmp_ptr != NULL)
    {
        delete_node_from_list(l, tmp_ptr);
    }
    add_id(l, id, mb);
}

static OpRes get_opres_from_str(char *s, bool is_correct)
{
    OpRes res;
    res.comment = s;
    res.is_correct_operation = is_correct;
    res.result = default_heap_type;
    return res;
}

static OpRes get_opres_from_heap_type(heap_type value)
{
    OpRes res;
    res.comment = EMPTY_STR;
    res.is_correct_operation = true;
    res.result = value;
    return res;
}

OpRes init_arr(const char *id, int lenght, heap_type default_value)
{
    if(lenght <= 0)
        return get_opres_from_str(INCORRECT_LENGHT_OF_ARRAY, false);
    add_id_correctly(id, get_array(lenght, default_value));
    return get_opres_from_str(CORRECT_OPERATION, true);
}

void put(char *id, heap_type value)
{
    init_arr(id, 1, value);
}

OpRes putarr(char *id, int cell, heap_type value)
{
    TableList *l = &table[hash_from_str(id)];
    ListNode *tmp_ptr = is_in_list(l, id);
    if(tmp_ptr == NULL)
        return get_opres_from_str(UNINICIALIZED_ARRAY, false);
    if(tmp_ptr->value.mem_block.lenght <= cell || cell < 0)
        return get_opres_from_str(INDEX_OUT_OF_RANGE_EXCEPTION, false);
    tmp_ptr->value.mem_block.value[cell] = value;
    return get_opres_from_str(CORRECT_OPERATION, true);
}

OpRes get(char *id)
{
    TableList *l = &table[hash_from_str(id)];
    ListNode *tmp_ptr = is_in_list(l, id);
    if(tmp_ptr == NULL)
        return get_opres_from_str(UNINICIALIZED_VALUE, false);
    return get_opres_from_heap_type(tmp_ptr->value.mem_block.value[0]);
}

OpRes getarr(char *id, int cell)
{
    TableList *l = &table[hash_from_str(id)];
    ListNode *tmp_ptr = is_in_list(l, id);
    if(tmp_ptr == NULL)
        return get_opres_from_str(UNINICIALIZED_ARRAY, false);
    if(tmp_ptr->value.mem_block.lenght <= cell || cell < 0)
        return get_opres_from_str(INDEX_OUT_OF_RANGE_EXCEPTION, false);
    return get_opres_from_heap_type(tmp_ptr->value.mem_block.value[cell]);
}

void print_opres(OpRes or)
{
    printf("Opres\n{\nComment: %s\nIs_correst: %s\nResult: %d\n}\n\n", 
    or.comment, or.is_correct_operation ? "true" : "false", or.result);
}

static void print_node(const ListNode *ptr)
{
    printf("Node {id: %s, value: ", ptr->value.id.str);
    if(ptr->value.mem_block.lenght == 1)
        printf("%d}\n", ptr->value.mem_block.value[0]);
    else if(ptr->value.mem_block.lenght > 1)
    {
        printf("{");
        for(int i = 0; i < ptr->value.mem_block.lenght; ++i)
            printf("%d%s", ptr->value.mem_block.value[i], i == ptr->value.mem_block.lenght - 1 ? "" : ", ");
        printf("}}\n");
    }
}

static void print_list(const TableList l, int num)
{
    if(l.length == 0)
        return;
    printf("list #%d\n{\n", num);
    ListNode *ptr = l.first;
    for(int i = 0; i < l.length; ++i, ptr = ptr->next)
        print_node(ptr);
    printf("}\n");
}

void print_all_variables(void)
{
    for(int i = 0; i < TABLE_SIZE; ++i)
    {
        if(table[i].length == 0)
            continue;
        
        print_list(table[i], i);
        printf("}\n");
    }
}