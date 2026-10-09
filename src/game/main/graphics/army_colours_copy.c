#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

extern u8 data_022a6c7c[5];

/* Copies the army colour of the five player slots into data_022a6c7c. */
void Player_SaveArmyColours(void)
{
    int i;

    for (i = 0; i < 5; i++) {
        data_022a6c7c[i] = STATE->players[i].armyColour;
    }
}
