#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* unk_07 holds 0-5. Values n and n + 3 form a pair (0/3, 1/4, 2/5) that
 * differ in the second option, which the menu offers once a game flag is
 * set (func_02047960). Keeps the pair and picks one of its two values. */
void Mode_SetVariant(int second)
{
    switch (data_022a7b80.unk_07) {
    case 0:
    case 3:
        if (second == 0) {
            data_022a7b80.unk_07 = 0;
        } else {
            data_022a7b80.unk_07 = 3;
        }
        break;
    case 2:
    case 5:
        if (second == 0) {
            data_022a7b80.unk_07 = 2;
        } else {
            data_022a7b80.unk_07 = 5;
        }
        break;
    default:
        if (second == 0) {
            data_022a7b80.unk_07 = 1;
        } else {
            data_022a7b80.unk_07 = 4;
        }
        break;
    }
}
