#include "types.h"
#include "game/Unk_02080440.h"

/* Battle menu entry test: 1 hides the entry. The "Music Off" label is a guess
 * from the check: shown while musicReady is 1 (music on). Handler Menu_ToggleMusic
 * writes musicReady. */
s8 BattleMenu_MusicOffHidden(void) { return MAP_STATE->musicReady != 1; }
