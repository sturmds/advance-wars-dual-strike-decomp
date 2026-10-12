#include "types.h"
#include "game/co_select_screen.h"
#include "sdk/os.h"

void* Mem_Alloc(u32 size);
void Mem_Free(void* ptr);
void Mem_Fill32(u32 value, void* dst, u32 size);
void Co_LoadTexture(int coId, u32 texOffset, u32 paletteOffset);
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size);
BOOL CoSelect_IsCoAvailable(u8 co);

/* Loads the picture of every CO in the CO list as a texture, one 0x180
 * byte image and one 32 byte palette per slot. A CO that is not available
 * gets a blank palette. The list ends with 0xfe; 0xff slots are skipped,
 * and the empty slot (0) is remembered in unk_37. */
void CoSelectScreen_LoadListTextures(void) {
  u8* co;
  int slot;
  void* blankPalette;
  int paletteOffset;
  int texOffset;

  slot = 0;
  co = data_0229c73c->coList;
  blankPalette = Mem_Alloc(32);

  Mem_Fill32(0, blankPalette, 32);
  DC_FlushRange(blankPalette, 32);
  if (*co != 0xfe) {
    paletteOffset = 0;
    texOffset = 0;
    do {
      if (*co == 0) {
        data_0229c73c->unk_37 = slot;
      } else if (*co != 0xff) {
        Co_LoadTexture(*co, texOffset, paletteOffset);
        if (CoSelect_IsCoAvailable(*co) == 0) {
          Gfx_LoadTexPalette(blankPalette, paletteOffset, 32);
        }
      }
      co++;
      paletteOffset += 32;
      texOffset += 0x180;
      slot++;
    } while (*co != 0xfe);
  }
  Mem_Free(blankPalette);
}
