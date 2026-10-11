#include "types.h"

extern void *data_027c027c;
extern void *data_027c0288;

void func_020d4748(void);
void MovePath_DrawArrows3d(void);
void func_020d44f0(void);

/* Runs func_020d4748 (it reads and edits the move path), then draws the
 * path with func_020d44f0, or with MovePath_DrawArrows3d when data_027c027c and
 * data_027c0288 hold the same pointer. The name is a guess. */
void MovePath_Draw(void)
{
    func_020d4748();
    if (data_027c027c != data_027c0288) {
        func_020d44f0();
    } else {
        MovePath_DrawArrows3d();
    }
}
