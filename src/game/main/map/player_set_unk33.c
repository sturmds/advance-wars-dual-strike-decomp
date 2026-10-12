#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets Player.unk_33, which the link checks (func_020cf17c) read. */
void Player_SetUnk33(int player) { STATE->players[player].unk_33 = 1; }
