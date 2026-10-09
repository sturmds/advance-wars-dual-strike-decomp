#include "types.h"
#include "game/force_rank_screen.h"
#include "game/skills.h"
#include "game/unlocks.h"

/* Fills the columns of the skills screen from the ov000 Force Rank table.
 * Columns 1-9 list the skills of every rank up to unk_7a. Column 10 lists
 * the special skills: Eagle Eye, Gear Head and Conqueror once the campaign
 * is beaten (flag 0x21), Mistwalker and Soul of Hachi once the hard campaign
 * is beaten (flag 0x22). Column 11 holds only skill 0, no skill. */
void ForceRankScreen_FillColumns(void)
{
    int column;
    int i; /* needed to match: one variable for the column loop and the skill id loop */

    for (i = 0; i < FORCE_RANK_COLUMN_COUNT; i++) {
        if (i == FORCE_RANK_COLUMN_NONE) {
            data_0229c738->columns[i].skills[0] = 0;
            data_0229c738->columns[i].count = 1;
        } else {
            data_0229c738->columns[i].count = 0;
        }
    }

    for (i = 0; i < 128; i++) {
        column = data_ov000_022f5ecc[i].column;
        if (column == 0) {
            continue;
        }
        if (column == FORCE_RANK_COLUMN_SPECIAL) {
            switch (i) {
            case SKILL_RANK_EAGLE_EYE:
            case SKILL_RANK_GEAR_HEAD:
            case SKILL_RANK_CONQUEROR:
                if (!GameFlag_IsSet(0x21)) {
                    continue;
                }
                break;
            case SKILL_RANK_MISTWALKER:
            case SKILL_RANK_SOUL_OF_HACHI:
                if (!GameFlag_IsSet(0x22)) {
                    continue;
                }
                break;
            }
            data_0229c738->columns[column].skills[data_0229c738->columns[column].count] = i;
            data_0229c738->columns[column].count++;
        } else if (column <= data_0229c738->unk_7a) {
            data_0229c738->columns[column].skills[data_0229c738->columns[column].count] = i;
            data_0229c738->columns[column].count++;
        }
    }
}
