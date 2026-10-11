#include "types.h"

/* Experience multiplier of a result row: bonus 1 gives 1.5 times points, 2
 * gives 2 times, 3 gives 2.5 times; anything else leaves it alone. The
 * EXP_BONUS_* constants in game/Unk_022a7a9c.h number these differently, so
 * the cases are plain numbers. */
int ExpBonus_Scale(int points, int bonus)
{
    switch (bonus) {
    case 1:
        return points + (points >> 1);
    case 2:
        return points << 1;
    case 3:
        return (points << 1) + (points >> 1);
    case 0:
    default:
        break;
    }
    return points;
}
