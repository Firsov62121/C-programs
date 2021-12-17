#include <stdio.h>
#include "my_heap.h"

int main(void)
{
    start_work();
    init_arr("ar1", 5, 3);
    print_opres(get("ar1"));
    print_opres(getarr("ar1", 1));
    print_opres(getarr("ar1", 5));
    print_opres(getarr("ar1", -1));
    print_opres(putarr("ar1", 4, 456));
    print_opres(getarr("ar1", 4));
    put("ar1", 1);
    print_opres(get("ar1"));

    put("12", 123);
    init_arr("21", 10, 45);
    print_all_variables();
    end_work();
    return 0;
}