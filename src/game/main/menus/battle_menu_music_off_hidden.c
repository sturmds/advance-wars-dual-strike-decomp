#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Battle menu entry test: 1 hides the entry. The "Music Off" label is a guess
 * from the check: shown while musicReady is 1 (music on). Handler Menu_ToggleMusic
 * writes musicReady. */
s8 BattleMenu_MusicOffHidden(void) { return STATE->musicReady != 1; }
