#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0235a30c.h"

#define GXFIFO_MTX_PUSH ((volatile u32*)0x04000444)
#define GXFIFO_MTX_POP ((volatile u32*)0x04000448)
#define GXFIFO_MTX_TRANS ((volatile u32*)0x04000470)

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void func_ov005_0235b2f0(Unk_ov005_0235a30c* work);
void func_ov005_0235b2cc(Unk_ov005_0235a30c* work);
void G3_RotY(int sin, int cos);
int Ov005_CharToGlyphIndex(u32 ch);
void Ov005_QueueSpritePair(void* work, int x, int y, int tile, u16 flags);
void Ov005_SendTexturedQuad(void* work, int x, int y, int z, int sizeS,
                            int sizeT, int width, int height, int texS,
                            int texT, u32 texAddr, u32 palAddr);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern s16 data_0214d4c4[];

/* Needed to match: the original walks the table with three counters (a moving pointer, a byte
 * offset and an unused index) and rebuilds the entry pointer from the offset for the glyphs.
 * Ov005_DrawCreditsEntryText has the same loop. */
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off
#pragma opt_dead_assignments off

/* `work` moved forward by `off` bytes: its entriesA[0] is entry off / sizeof(entry) of the table. */
#define WORK_AT(off) ((Unk_ov005_0235a30c*)((u8*)work + (off)))

/* Draws the first entry table on the 3D screen. An entry with id 2 is queued as a sprite pair,
 * centred by its length. Every other entry draws 32 glyph quads 8 pixels apart, each turned
 * about the Y axis by the entry's angle; positions past the text show glyph 31. Styles 2 and 3
 * take their glyphs from the second row of the font texture (t = 32). */
void Ov005_DrawCreditsEntries3D(Unk_ov005_0235a30c* work) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;
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
  Unk_ov005_0235a30c* row;
  int off;
  Unk_ov005_0235a30c* entry;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_ov005_0235b2f0(work);
  for (i = 0, row = work, off = 0;; i++) {
    if (row->entriesA[0].id == 0) {
      break;
    }
    angle = row->entriesA[0].angle;
    x = row->entriesA[0].x;
    y = row->entriesA[0].y;
    tile = row->entriesA[0].tile;
    if (row->entriesA[0].id == 2) {
      Ov005_QueueSpritePair(
          work, (s16)((256 - row->entriesA[0].length * 8) >> 1), y, tile, 0);
    } else {
      idx = (angle >> 4) * 2;
      quadY = y - 96;
      j = 0;
      *GXFIFO_MTX_PUSH = 0;
      cos = data_0214d4c4[idx + 1];
      sin = data_0214d4c4[idx];
      entry = WORK_AT(off);
      for (; j < CREDITS_ENTRY_GLYPHS; j++) {
        *GXFIFO_MTX_PUSH = 0;
        G3_RotY(sin, cos);
        *GXFIFO_MTX_TRANS = x;
        *GXFIFO_MTX_TRANS = quadY;
        *GXFIFO_MTX_TRANS = 0;
        if (j < entry->entriesA[0].length) {
          glyph = Ov005_CharToGlyphIndex(entry->entriesA[0].text[j]);
        } else {
          glyph = 31;
        }
        texT = 0;
        if (entry->entriesA[0].style == 2) {
          texT = 32;
        } else if (entry->entriesA[0].style == 3) {
          texT = 32;
        }
        Ov005_SendTexturedQuad(work, 0, 0, 0, 5, 3, 8, 16, (u16)(glyph * 8),
                               texT, 0, 0);
        *GXFIFO_MTX_POP = 1;
        x += 8;
      }
      *GXFIFO_MTX_POP = 1;
    }
    row = (Unk_ov005_0235a30c*)((u8*)row + sizeof(Unk_ov005_0235abfc));
    off += sizeof(Unk_ov005_0235abfc);
  }
  func_ov005_0235b2cc(work);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
