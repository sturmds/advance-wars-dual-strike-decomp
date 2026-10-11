#include "types.h"

void CoRankMarker_Start(int x, int y, int arg);

/* Starts the marker task (CoRankMarker_Start) for one CO: front CO at x 32, back
 * CO at x 168. The third argument ends up in the task's unk_52. */
void CoRankScreen_StartMarkerTask(int coIndex)
{
    int arg = 31 - coIndex;

    if (coIndex == 0) {
        CoRankMarker_Start(32, 140, arg);
    } else {
        CoRankMarker_Start(168, 140, arg);
    }
}
