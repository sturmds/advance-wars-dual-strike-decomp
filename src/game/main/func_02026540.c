#include "types.h"

void func_02026870(int arg);   /* empty */
void func_0202686c(void);      /* empty */

/* Called once from game init, next to func_02026874. Both callees are
 * empty in the retail ROM. */
void func_02026540(void)
{
    func_02026870(3);
    func_0202686c();
}
