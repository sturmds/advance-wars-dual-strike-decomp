#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/Unk_022a7b80.h"

u32 Co_GetExperience(int co);
void Co_SetExperience(int co, u32 experience);
u32 Survival_GetExperienceBonus(void);
void PlayRecords_UpdateCoRankTotal(void);

extern Unk_022a7b80 data_022a7b80;

#define EXPERIENCE_MAX_GAIN 9999

/* Adds experience to the two COs of player 1's result row (front and back,
 * when set): the run's points (data_022a7b80.points) plus the mode's bonus
 * from Survival_GetExperienceBonus, at most 9999. Then PlayRecords_UpdateCoRankTotal
 * updates the stored total of CO ranks. */
void Survival_AwardCoExperience(void)
{
    int gain;
    int co;
    int i; /* declaration order needed to match */
    u32 total;

    for (i = 0; i < 2; i++) {
        co = (&((Unk_022a7a9c *)&data_022a7a9c)[1].frontCo)[i] & 0x7f;
        if (co != 0) {
            gain = data_022a7b80.points + Survival_GetExperienceBonus();
            if (gain > EXPERIENCE_MAX_GAIN) {
                gain = EXPERIENCE_MAX_GAIN;
            }
            total = gain + Co_GetExperience(co);
            Co_SetExperience(co, total);
        }
    }
    PlayRecords_UpdateCoRankTotal();
}
