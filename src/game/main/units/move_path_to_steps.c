#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

extern MovePath *data_02166e7c;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Turns the drawn move path into one step per tile (PATH_STEP_*) in
 * unk_170, ending with PATH_STEP_END. */
void MovePath_ToSteps(void)
{
    s8 i;

    for (i = 1; i <= data_02166e7c->lastTile; i++) {
        if (data_02166e7c->x[i] < data_02166e7c->x[i - 1]) {
            data_027c027c->unk_48[data_027c0284]->unk_170[i - 1] = PATH_STEP_LEFT;
        } else if (data_02166e7c->x[i] > data_02166e7c->x[i - 1]) {
            data_027c027c->unk_48[data_027c0284]->unk_170[i - 1] = PATH_STEP_RIGHT;
        } else if (data_02166e7c->y[i] < data_02166e7c->y[i - 1]) {
            data_027c027c->unk_48[data_027c0284]->unk_170[i - 1] = PATH_STEP_UP;
        } else {
            data_027c027c->unk_48[data_027c0284]->unk_170[i - 1] = PATH_STEP_DOWN;
        }
    }
    data_027c027c->unk_48[data_027c0284]->unk_170[i - 1] = PATH_STEP_END;
}
