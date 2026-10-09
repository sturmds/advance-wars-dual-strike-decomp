#include "types.h"
#include "game/script_slot.h"

extern ScriptCommand data_0215837c[];
extern ScriptCommand data_02158394[];
extern ScriptCommand data_021583b4[];

s8 func_02017130(const ScriptCommand *script);   /* slot running script, or -1 */

/* TRUE while any of the three text printing scripts runs in a script slot. */
BOOL Text_IsPrinting(void)
{
    int running = 0;

    running += func_02017130(data_0215837c) != -1;
    running += func_02017130(data_02158394) != -1;
    running += func_02017130(data_021583b4) != -1;
    return running > 0;
}
