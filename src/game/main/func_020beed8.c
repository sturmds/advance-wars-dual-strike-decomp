#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

int func_020bef88(void);

/* Returns 1 when the acting unit is not a Stealth, else func_020bef88(). */
int func_020beed8(void)
{
    /* A one-case switch, needed to match (an if gives conditional returns). */
    switch (data_027c027c->unk_48[data_027c0284]->actingUnit->type) {
    case UNIT_STEALTH:
        return func_020bef88();
    }
    return 1;
}
