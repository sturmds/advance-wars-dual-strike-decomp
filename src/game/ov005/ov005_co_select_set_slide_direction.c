#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

/* Sets which way the slide task moves after the cursor moved. unk_18 and unk_1c
 * hold the previous cell (guess). A jump of more than 20 cells counts as the
 * other way round, as does a wrap. */
void Ov005CoSelect_SetSlideDirection(Unk_ov005_0236bbac *state)
{
    Ov005CoSlideTask *task = state->unk_50;
    int oldIndex;
    int newIndex;

    task->step = 1;
    oldIndex = state->unk_18 + state->unk_1c * 8;
    newIndex = state->selection + state->page * 8;
    if (state->unk_14 == 0) {
        if (oldIndex < newIndex) {
            if (newIndex - oldIndex > 20) {
                task->slideBack = 1;
            } else {
                task->slideBack = 0;
            }
        } else {
            if (oldIndex - newIndex > 20) {
                task->slideBack = 0;
            } else {
                task->slideBack = 1;
            }
        }
    } else if (state->unk_14 == 1) {
        task->slideBack = 1;
    } else {
        task->slideBack = 0;
    }
}
