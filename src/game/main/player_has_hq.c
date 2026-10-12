#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* TRUE when the map has an HQ tile for the player. Map_CountPlayerProperties sets bit 7
 * of hqX for every player and clears it when it stores the HQ position. */
BOOL Player_HasHq(int player) { return (PLAYERS[player].hqX & 0x80) == 0; }
