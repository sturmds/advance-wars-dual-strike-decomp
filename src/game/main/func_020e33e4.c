#include "types.h"

void func_020e340c(void);
extern u8 data_022a7294;

void * func_020e33e4(void)
{
    func_020e340c();
    data_022a7294 = 0;
    return &data_022a7294;
}
