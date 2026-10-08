#include "types.h"

/* Tests skill number `skill` (1-based) in a skill bitfield such as
 * Player.frontSkills. Skill 0 means no skill and is never set. */
BOOL CheckSkillsEquipped(const u32 *bits, int skill)
{
    int bit;
    BOOL set;

    if (skill == 0) {
        return FALSE;
    }
    bit = skill - 1;
    set = TRUE;
    if ((bits[(bit & 0xe0) >> 5] & (1 << (bit & 0x1f))) == 0) {
        set = FALSE;
    }
    return set;
}
