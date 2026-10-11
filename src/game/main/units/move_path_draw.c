#include "types.h"

extern void *data_027c027c;
extern void *data_027c0288;

void MovePath_FollowCursor(void);
void MovePath_DrawArrows3d(void);
void MovePath_DrawArrowSprites(void);

/* Runs MovePath_FollowCursor (it reads and edits the move path), then draws the
 * path with MovePath_DrawArrowSprites, or with MovePath_DrawArrows3d when data_027c027c and
 * data_027c0288 hold the same pointer. The name is a guess. */
void MovePath_Draw(void)
{
    MovePath_FollowCursor();
    if (data_027c027c != data_027c0288) {
        MovePath_DrawArrowSprites();
    } else {
        MovePath_DrawArrows3d();
    }
}
