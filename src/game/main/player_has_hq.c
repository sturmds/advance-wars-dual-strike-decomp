#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* TRUE when the map has an HQ tile for the player. func_020c0aa8 sets bit 7
 * of unk_2f for every player and clears it when it stores the HQ position. */
BOOL Player_HasHq(int player)
{
    return (PLAYERS[player].unk_2f & 0x80) == 0;
}
