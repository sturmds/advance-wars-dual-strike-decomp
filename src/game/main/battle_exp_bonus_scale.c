#include "types.h"
#include "game/Unk_022a7a9c.h"

/* Experience multiplier of a result row (EXP_BONUS_*); an unknown value
 * leaves points alone. */
int ExpBonus_Scale(int points, int bonus)
{
    switch (bonus) {
    case EXP_BONUS_X1_5:
        return points + (points >> 1);
    case EXP_BONUS_X2:
        return points << 1;
    case EXP_BONUS_X2_5:
        return (points << 1) + (points >> 1);
    case EXP_BONUS_X1:
    default:
        break;
    }
    return points;
}
