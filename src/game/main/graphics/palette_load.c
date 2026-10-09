#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

void func_02003ca0(const void *src, void *dst, u32 size);   /* copies in 16-bit units */
void func_020031bc(void);                                   /* sends the palette buffer to palette RAM */

/* Copies size bytes of colours into the current screen's palette buffer,
 * offset bytes in (0x200 starts the OBJ palettes), then uploads it. */
void Palette_Load(const void *src, int offset, u32 size)
{
    func_02003ca0(src, (u16 *)data_027c0280->unk_08 + (offset >> 1), size);
    func_020031bc();
}
