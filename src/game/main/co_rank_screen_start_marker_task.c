#include "types.h"

void func_02049070(int x, int y, int arg);

/* Starts the marker task (func_02049070) for one CO: front CO at x 32, back
 * CO at x 168. The third argument ends up in the task's unk_52. */
void CoRankScreen_StartMarkerTask(int coIndex)
{
    int arg = 31 - coIndex;

    if (coIndex == 0) {
        func_02049070(32, 140, arg);
    } else {
        func_02049070(168, 140, arg);
    }
}
