#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Sets unk_0d; func_020b9864 clears it. Called when START leaves the CO
 * select screen and when a map starts in its CO select phase. */
void func_020b9890(void) { data_027c027c->unk_48[data_027c0284]->unk_0d = 1; }
