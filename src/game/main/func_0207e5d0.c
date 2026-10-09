#include "types.h"

/* Adds (id, arg) to a 16-entry ring queue at data_0229d89c. */
void EventQueue_Push(int id, int arg);

void func_0207e5d0(void)
{
    EventQueue_Push(2, 0);
}
