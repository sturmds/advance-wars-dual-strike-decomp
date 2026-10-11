#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Battle menu entry test: 1 hides the entry. The "Music On" label is a guess
 * from the check: shown while musicReady is 0 (music off). Handler func_020bd940
 * writes musicReady. */
s8 BattleMenu_MusicOnHidden(void)
{
    return STATE->musicReady != 0;
}
