#include "types.h"

int func_02007748(void);
int func_02007730(void);
void func_02007710(int a0);
void Screen_SetBothNibbles(int a0);
void Task_Destroy(void *task);

int func_02003d10(void *task)
{
    if (func_02007748() == 0) {
        if (func_02007730() == 0) {
            Task_Destroy(task);
            return 0;
        }
        func_02007710(3);
    } else if (func_02007730() == 0) {
        func_02007710(2);
    } else {
        func_02007710(1);
    }
    Screen_SetBothNibbles(0);
    return 1;
}
