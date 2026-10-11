#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0235a30c.h"

#define GXFIFO_MTX_PUSH ((volatile u32 *)0x04000444)
#define GXFIFO_MTX_POP ((volatile u32 *)0x04000448)
#define GXFIFO_MTX_TRANS ((volatile u32 *)0x04000470)

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void func_ov005_0235b2f0(Unk_ov005_0235a30c *work);
void func_ov005_0235b2cc(Unk_ov005_0235a30c *work);
void G3_RotY(int sin, int cos);
int Ov005_CharToGlyphIndex(u32 ch);
void Ov005_QueueSpritePair(void *work, int x, int y, int tile, u16 flags);
void Ov005_SendTexturedQuad(void *work, int x, int y, int z, int sizeS, int sizeT, int width,
                            int height, int texS, int texT, u32 texAddr, u32 palAddr);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern s16 data_0214d4c4[];

/* Entry of table A at byte offset `off` from the first. Needed to get close: the glyph loop
 * reads the entry through a byte offset kept beside i, as the original does. */
#define ENTRY_AT(off) (((Unk_ov005_0235a30c *)((u8 *)work + (off)))->entriesA[0])

/* 97.5%: the original keeps the byte offset in the stack slot after the compiler's entry
 * pointer; ours has the two slots swapped (4 instructions). Likely the original's offset
 * came from the compiler too, from work->entriesA[i] in the glyph loop.
 *
 * Draws the first entry table on the 3D screen. An entry with id 2 is queued as a sprite pair,
 * centred by its length. Every other entry draws 32 glyph quads 8 pixels apart, each turned
 * about the Y axis by the entry's angle; positions past the text show glyph 31. Styles 2 and 3
 * take their glyphs from the second row of the font texture (t = 32). */
void func_ov005_0235aec8(Unk_ov005_0235a30c *work)
{
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedScreen;
    int i;
    int j;
    int x;
    int y;
    int idx;
    int cos;
    int sin;
    u16 glyph;
    int texT;
    int angle;
    int tile;
    int quadY;
    int off;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_ov005_0235b2f0(work);
    for (i = 0, off = 0;; off += sizeof(Unk_ov005_0235abfc), i++) {
        if (work->entriesA[i].id == 0) {
            break;
        }
        angle = work->entriesA[i].angle;
        x = work->entriesA[i].x;
        y = work->entriesA[i].y;
        tile = work->entriesA[i].tile;
        if (work->entriesA[i].id == 2) {
            Ov005_QueueSpritePair(work, (s16)((256 - work->entriesA[i].length * 8) >> 1), y, tile, 0);
        } else {
            idx = (angle >> 4) * 2;
            quadY = y - 96;
            j = 0;
            *GXFIFO_MTX_PUSH = 0;
            cos = data_0214d4c4[idx + 1];
            sin = data_0214d4c4[idx];
            for (; j < 32; j++) {
                *GXFIFO_MTX_PUSH = 0;
                G3_RotY(sin, cos);
                *GXFIFO_MTX_TRANS = x;
                *GXFIFO_MTX_TRANS = quadY;
                *GXFIFO_MTX_TRANS = 0;
                if (j < ENTRY_AT(off).length) {
                    glyph = Ov005_CharToGlyphIndex(ENTRY_AT(off).text[j]);
                } else {
                    glyph = 31;
                }
                texT = 0;
                if (ENTRY_AT(off).style == 2) {
                    texT = 32;
                } else if (ENTRY_AT(off).style == 3) {
                    texT = 32;
                }
                Ov005_SendTexturedQuad(work, 0, 0, 0, 5, 3, 8, 16, (u16)(glyph * 8), texT, 0, 0);
                *GXFIFO_MTX_POP = 1;
                x += 8;
            }
            *GXFIFO_MTX_POP = 1;
        }
    }
    func_ov005_0235b2cc(work);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
}
