#include "types.h"

u8 (*GetCoPicksForMode(int mode))[2];

extern u8 gCoPicksDefault[5][2];
extern u8 data_02167d30[5][2];

/* Resets the CO picks of players 1-4 in all four modes: Versus (mode 2) to
 * data_02167d30, the others to gCoPicksDefault. */
void CoPicks_ResetToDefaults(void)
{
    u8 (*mode0)[2] = GetCoPicksForMode(0);
    u8 (*mode1)[2] = GetCoPicksForMode(1);
    u8 (*mode2)[2] = GetCoPicksForMode(2);
    u8 (*mode3)[2] = GetCoPicksForMode(3);
    int player;
    int slot;

    for (player = 1; player < 5; player++) {
        for (slot = 0; slot < 2; slot++) {
            mode0[player][slot] = gCoPicksDefault[player][slot];
            mode1[player][slot] = gCoPicksDefault[player][slot];
            mode2[player][slot] = data_02167d30[player][slot];
            mode3[player][slot] = gCoPicksDefault[player][slot];
        }
    }
}
