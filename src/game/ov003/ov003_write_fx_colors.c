#include "types.h"
#include "game/Ov003Fx.h"

void Mem_CopyUnits(const void *src, void *dst, u32 size, u32 unitBits);
/* LCDC address of colour 13 in row i of the 16-colour rows in bank F. */
#define BANK_F_COLOR(i) ((void *)(0x06890000 + 0x1a + (i) * 0x20 + 0x80))

void GX_BeginLoadTexPltt(void);   /* guess: maps VRAM bank F to the CPU (LCDC) */

void GX_EndLoadTexPltt(void);   /* guess: undoes GX_BeginLoadTexPltt */

/* Writes the two colours into bank F, at palette entry 13 of four 16-colour
 * rows. Mode 11 sends the first 4 bytes of the state instead (kept as the
 * original reads it). */
void Ov003_WriteFxColors(void)
{
    GX_BeginLoadTexPltt();
    if (data_ov003_02364a84->mode == 0) {
        Mem_CopyUnits(&data_ov003_02364a84->colors, BANK_F_COLOR(0), 4, 16);
        Mem_CopyUnits(&data_ov003_02364a84->colors, BANK_F_COLOR(1), 4, 16);
        Mem_CopyUnits(&data_ov003_02364a84->colors, BANK_F_COLOR(2), 4, 16);
        Mem_CopyUnits(&data_ov003_02364a84->colors, BANK_F_COLOR(3), 4, 16);
    } else if (data_ov003_02364a84->mode == 11) {
        Mem_CopyUnits(data_ov003_02364a84, BANK_F_COLOR(0), 4, 16);
        Mem_CopyUnits(data_ov003_02364a84, BANK_F_COLOR(1), 4, 16);
        Mem_CopyUnits(data_ov003_02364a84, BANK_F_COLOR(2), 4, 16);
        Mem_CopyUnits(data_ov003_02364a84, BANK_F_COLOR(3), 4, 16);
    }
    GX_EndLoadTexPltt();
}
