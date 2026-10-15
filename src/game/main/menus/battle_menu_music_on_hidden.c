#include "types.h"
#include "game/Unk_02080440.h"

/* Battle menu entry test: 1 hides the entry. The "Music On" label is a guess
 * from the check: shown while musicReady is 0 (music off). Handler Menu_ToggleMusic
 * writes musicReady. */
s8 BattleMenu_MusicOnHidden(void) { return MAP_STATE->musicReady != 0; }
