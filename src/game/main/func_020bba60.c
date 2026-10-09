#include "types.h"

void Weather_StartEffect(int weather);
void MenuButtons_CloseAll(void);
void MenuDepth_Pop(void);

void func_020bba60(void)
{
    Weather_StartEffect(1);
    MenuButtons_CloseAll();
    MenuDepth_Pop();
}
