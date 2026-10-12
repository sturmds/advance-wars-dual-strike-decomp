#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Clears bit 1 of unk_5b in the current game state; func_020c0704 sets it.
 * What the bit controls is unknown. */
void func_020c0734(void) { data_027c027c->unk_48[data_027c0284]->unk_5b &= ~2; }
