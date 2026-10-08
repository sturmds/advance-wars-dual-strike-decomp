#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Sets unk_ba; 1 turns off the Scout and Eagle Eye vision bonus
 * (func_020e8ae4, VisionIncrease). */
void func_020e8a88(u8 value)
{
    data_027c027c->unk_48[data_027c0284]->unk_ba = value;
}
