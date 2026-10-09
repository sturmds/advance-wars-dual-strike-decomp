#include "types.h"

void func_020e7e90(u32 *skills); /* clears a 4-word skill set; suggested name Skills_Clear */
void PartnerBoost2(u32 *skills, int id);

/* Clears a skill set, then turns on the ids in `list` (up to 4, ended by 0). */
void Skills_SetFromList(const u8 *list, u32 *skills)
{
    int i;

    func_020e7e90(skills);
    if (list == NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (list[i] == 0) {
            return;
        }
        PartnerBoost2(skills, list[i]);
    }
}
