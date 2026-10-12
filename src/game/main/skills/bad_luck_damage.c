#include "types.h"

/* Bad luck added by the skills of one CO (coSlot 0 front, 1 back) of a
 * player. No skill adds any in retail, so this always returns 0. Bad luck
 * counterpart of LuckDamage. */
int BadLuckDamage(int playerIndex, int coSlot) { return 0; }
