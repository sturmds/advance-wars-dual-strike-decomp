#include "types.h"

int func_020960cc(void);
int func_02095fe8(void);

/* Picks the airport site to build a Black Bomb at: the one with the most
 * enemies near it, or when none qualifies, the one nearest an enemy.
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirport(void)
{
    int index;

    index = func_020960cc();
    if (index != -1) {
        return index;
    }
    return func_02095fe8();
}
