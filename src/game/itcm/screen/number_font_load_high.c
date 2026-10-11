#include "types.h"
#include "game/screen_other_map.h"

extern u8 data_ov000_023503a0;              /* game language */
extern const void *data_ov000_022f6638[];   /* packed number font graphics per language */
extern u8 data_02138c68[];                  /* its palette */

void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, u32 offset, u32 size);

#define NUMBER_FONT_HIGH_OFFSET 0xf600      /* object tile 0x7b0 */
#define NUMBER_FONT_PALETTE_OFFSET 0x3e0    /* sprite palette 15 */

/* Loads the number font graphics (NumberSpriteTask_Start loads the same
 * palette) into the current screen's object tiles, at the high slot. */
void NumberFont_LoadHigh(void)
{
    Data_Unpack(data_ov000_022f6638[data_ov000_023503a0], data_027c0280->objVram + NUMBER_FONT_HIGH_OFFSET);
    Palette_Load(data_02138c68, NUMBER_FONT_PALETTE_OFFSET, 32);
}
