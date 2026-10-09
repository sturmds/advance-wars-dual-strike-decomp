#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void func_0210a46c(int displayMode, int bgMode, int bg0Is3d);   /* SDK GX_SetGraphicsMode (main engine) */
void func_0210a450(int bgMode);                                  /* SDK GXS_SetGraphicsMode (sub engine) */

/* Sets the BG mode of the current screen and applies it to the main or sub
 * engine, depending on which screen is current. */
void Screen_SetBgMode(int bgMode)
{
    GFX_BG_MODE(data_027c0280) = bgMode;
    if (data_027c027c == data_027c0288.unk_00) {
        func_0210a46c(data_027c0280->dispMode, GFX_BG_MODE(data_027c0280), data_027c0280->bg0Mode);
    } else {
        func_0210a450(GFX_BG_MODE(data_027c0280));
    }
}
