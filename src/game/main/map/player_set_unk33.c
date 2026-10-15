#include "types.h"
#include "game/Unk_02080440.h"

/* Sets Player.unk_33, which the link checks (func_020cf17c) read. */
void Player_SetUnk33(int player) { MAP_STATE->players[player].unk_33 = 1; }
