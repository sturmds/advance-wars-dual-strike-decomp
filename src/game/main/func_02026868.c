#include "types.h"

/* Three functions that are empty in the retail ROM, next to the song code.
 * func_0203295c calls func_02026868 with 9; func_02026540 calls
 * func_02026870 with 3, then func_0202686c.
 * Needed to match: written in reverse address order, because the compiler
 * emits them last to first. */

void func_02026870(int arg) {}

void func_0202686c(void) {}

void func_02026868(int arg) {}
