#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

int func_020f35cc(void);

/* TRUE in a two-map battle when the other map's unk_1c4->unk_30 is 0 (the
 * same test as func_020f368c), otherwise when func_020f35cc returns 1 or 2.
 * Purpose unknown; Game_State15_FreeCursor passes the result to MapTouch_ReadButtons. */
BOOL func_020d194c(void)
{
    int value;

    if (STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
        return TRUE;
    }
    value = func_020f35cc();
    return value == 1 || value == 2;
}
