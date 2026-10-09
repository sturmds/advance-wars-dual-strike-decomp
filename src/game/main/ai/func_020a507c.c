#include "types.h"

int func_020a5164(int x, int y, int range, int arg3);
int func_020a6260(int a, int b, int c);

/* Callback the path search calls for each tile. */
extern int (*data_0229e174)(int a, int b, int c);

int func_020a507c(int x, int y, int range)
{
    data_0229e174 = func_020a6260;
    if (data_0229e174(x, y, 0) == 1) {
        return 1;
    }
    return func_020a5164(x, y, 0, range);
}
