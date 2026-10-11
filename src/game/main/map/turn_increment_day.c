#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Adds 1 to the current map state's unk_00 (likely the day), at most 999. */
void Turn_IncrementDay(void)
{
    if (data_027c027c->unk_48[data_027c0284]->unk_00 < 999) {
        data_027c027c->unk_48[data_027c0284]->unk_00++;
    }
}
