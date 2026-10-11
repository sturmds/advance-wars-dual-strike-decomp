#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;
extern u8 gMenuState[];
extern u16 *data_ov003_02364a84;
extern u8 data_ov003_02365d10[];
extern char data_ov003_023649f4[];
extern u16 data_ov003_02366110[];
extern u16 data_ov003_02367110[];

void Data_Unpack(const void *src, void *dst);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Mem_CopyUnits(const void *src, void *dst, u32 size, u32 unitBits);
void Palette_Load(const void *src, u32 offset, u32 size);
void File_UnpackTo(void *dest, const char *fileName);
void GX_EndLoadTexPltt(void);
void GX_BeginLoadTexPltt(void);
void GX_EndLoadTex(void);
void GX_BeginLoadTex(void);

#define STATE data_ov003_02364928
#define GFX data_027c0280
#define PALETTE ((u16 *)GFX->paletteBuffer)
#define BG0_CHARS (GFX->bgVram + (GFX->bgCnt[GFX->layer0Bg].charBase << 14))

/* VRAM as the CPU sees it: sub screen sprites, and banks A and F while
 * mapped to the CPU (LCDC). */
#define VRAM_SUB_OBJ 0x06600000
#define VRAM_LCDC_A  0x06800000
#define VRAM_LCDC_F  0x06890000

/* Screen entry flag: BG palette 2. */
#define TILE_PALETTE_2 0x2000

/* Loads the overlay's graphics from the resource res. Each row in use gets the
 * BG tiles, palette colours, bank A data and bank F palette of its colour
 * (Ov003Row.color); gMenuState serves as the unpack buffer. Also loads the
 * sprites, the tile code table data_ov003_02365d10 from a file, and two maps
 * of 32 x 16 metatiles of 2 x 2 tiles into data_ov003_02367110 and
 * data_ov003_02366110. Needed to
 * match: one pointer, buf, views gMenuState for the palette copy and both
 * metatile loops. */
void Ov003_LoadGraphics(u8 *res)
{
    int i;
    u16 *buf;
    int topLeft;

    Data_Unpack(res, gMenuState);
    for (i = 0; i < STATE->rowCount; i++) {
        if (i & 1) {
            Mem_Copy32(gMenuState + ((STATE->rows[i].color - 1) * 16 + 8) * 32,
                       BG0_CHARS + (0x260 + i * 8) * 32, 0x100);
        } else {
            Mem_Copy32(gMenuState + (STATE->rows[i].color - 1) * 512,
                       BG0_CHARS + (0x260 + i * 8) * 32, 0x100);
        }
        Mem_Copy32(gMenuState + (STATE->rows[i].color - 1) * 512,
                   BG0_CHARS + (0x300 + i * 8) * 32, 0x100);
    }

    Data_Unpack(res + 0x6dc, (void *)(VRAM_SUB_OBJ + 0x1e000));
    Palette_Load(res + 0x1384, 0x40, 0x180);
    /* Keep the colours rows 0-3 had, in groups of four (group 4 from
     * palette 8), then give each of those rows the group of its colour. */
    buf = (u16 *)gMenuState;
    buf[0] = PALETTE[0x95];
    buf[1] = PALETTE[0x97];
    buf[2] = PALETTE[0x99];
    buf[3] = PALETTE[0x9b];
    buf[4] = PALETTE[0x96];
    buf[5] = PALETTE[0x98];
    buf[6] = PALETTE[0x9a];
    buf[7] = PALETTE[0x9c];
    buf[8] = PALETTE[0xa5];
    buf[9] = PALETTE[0xa7];
    buf[10] = PALETTE[0xa9];
    buf[11] = PALETTE[0xab];
    buf[12] = PALETTE[0xa6];
    buf[13] = PALETTE[0xa8];
    buf[14] = PALETTE[0xaa];
    buf[15] = PALETTE[0xac];
    buf[16] = PALETTE[0x86];
    buf[17] = PALETTE[0x88];
    buf[18] = PALETTE[0x8a];
    buf[19] = PALETTE[0x8c];
    for (i = 0; i < 4; i++) {
        PALETTE[0x95 + i * 2] = buf[(STATE->rows[0].color - 1) * 4 + i];
    }
    for (i = 0; i < 4; i++) {
        PALETTE[0x96 + i * 2] = buf[(STATE->rows[1].color - 1) * 4 + i];
    }
    for (i = 0; i < 4; i++) {
        PALETTE[0xa5 + i * 2] = buf[(STATE->rows[2].color - 1) * 4 + i];
    }
    for (i = 0; i < 4; i++) {
        PALETTE[0xa6 + i * 2] = buf[(STATE->rows[3].color - 1) * 4 + i];
    }

    Mem_CopyUnits(res + 0x149e, data_ov003_02364a84, 4, 16);
    Mem_CopyUnits(res + 0x151e, data_ov003_02364a84 + 2, 4, 16);
    Data_Unpack(res + 0x1bf0, GFX->objVram);
    Data_Unpack(res + 0x21fc, GFX->objVram + 0x3000);
    Data_Unpack(res + 0x2438, GFX->objVram + 0x4000);
    Data_Unpack(res + 0x28b4, GFX->objVram + 0x6200);

    GX_BeginLoadTex();
    Data_Unpack(res + 0x1524, (void *)VRAM_LCDC_A);
    Mem_Copy32((void *)(VRAM_LCDC_A + 0x100), gMenuState, 0x280);
    for (i = 0; i < STATE->rowCount; i++) {
        Mem_Copy32(gMenuState + (STATE->rows[i].color - 1) * 128, (void *)(((8 + i * 4) << 5) + VRAM_LCDC_A), 0x80);
    }
    GX_EndLoadTex();

    GX_BeginLoadTexPltt();
    Mem_Copy32(res + 0x1a90, (void *)VRAM_LCDC_F, 0x160);
    Mem_Copy32(res + 0x1b10, gMenuState, 0xa0);
    for (i = 0; i < STATE->rowCount; i++) {
        Mem_Copy32(gMenuState + (STATE->rows[i].color - 1) * 32, (u8 *)VRAM_LCDC_F + (i + 4) * 32, 0x20);
    }
    GX_EndLoadTexPltt();

    File_UnpackTo(data_ov003_02365d10, data_ov003_023649f4);

    /* buf holds two 32 x 32 tile maps side by side: metatiles 0-15 of a row
     * come from the first, 16-31 from the second. */
    Data_Unpack(res + 0x770, gMenuState);
    buf = (u16 *)gMenuState;

    for (i = 0; i < 512; i++) {
        if (!(i & 16)) {
            topLeft = (i >> 5) * 64 + (i & 15) * 2;
            data_ov003_02367110[i * 4 + 0] = buf[topLeft] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 1] = buf[topLeft + 1] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 2] = buf[topLeft + 32] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 3] = buf[topLeft + 33] + TILE_PALETTE_2;
        } else {
            data_ov003_02367110[i * 4 + 0] = buf[(i >> 5) * 64 + 0x400 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 1] = buf[(i >> 5) * 64 + 0x401 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 2] = buf[(i >> 5) * 64 + 0x420 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02367110[i * 4 + 3] = buf[(i >> 5) * 64 + 0x421 + (i & 15) * 2] + TILE_PALETTE_2;
        }
    }

    Data_Unpack(res + 0x1078, gMenuState);
    buf = (u16 *)gMenuState;
    for (i = 0; i < 512; i++) {
        if (!(i & 16)) {
            topLeft = (i >> 5) * 64 + (i & 15) * 2;
            data_ov003_02366110[i * 4 + 0] = buf[topLeft] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 1] = buf[topLeft + 1] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 2] = buf[topLeft + 32] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 3] = buf[topLeft + 33] + TILE_PALETTE_2;
        } else {
            data_ov003_02366110[i * 4 + 0] = buf[(i >> 5) * 64 + 0x400 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 1] = buf[(i >> 5) * 64 + 0x401 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 2] = buf[(i >> 5) * 64 + 0x420 + (i & 15) * 2] + TILE_PALETTE_2;
            data_ov003_02366110[i * 4 + 3] = buf[(i >> 5) * 64 + 0x421 + (i & 15) * 2] + TILE_PALETTE_2;
        }
    }
}
