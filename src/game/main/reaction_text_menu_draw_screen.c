#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/reaction_text_menu_task.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_LoadBmap09f(void* dst, int unused, int paletteSlot);
void Font_LoadDigits(int baseTile);
u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, int offset, u32 size);
void Mem_Free(void* file);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern char data_0215fcd4[]; /* reaction text menu graphics file */
extern char data_0229da8c[][REACTION_TEXT_SIZE]; /* the six reaction texts */

#define REACTION_TEXT_COUNT 6

/* Draws the reaction text menu screen: layers 1 and 2 from one file, the
 * six reaction texts on layer 0 (rows 6 to 16), BG0-3 and OBJ on, with
 * layer 2 in front. The menu's script (data_0215fc0c) and the
 * data_0215fc44 script both run it. */
void ReactionTextMenu_DrawScreen(void) {
  u8* file;
  int i;

  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_ClearLayerMap(2);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  file = File_Load(data_0215fcd4);
  Data_Unpack(file + 0x2ac, data_027c0280->layer1Map);
  Data_Unpack(
      file, data_027c0280->bgVram +
                (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Data_Unpack(file + 0x19c, data_027c0280->layer2Map);
  Palette_Load(file + 0x28c, 0x40, 0x20);
  Mem_Free(file);
  /* Layer 2 map entries use palette 2, where the file's palette went. */
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x2000;
  }
  for (i = 0; i < REACTION_TEXT_COUNT; i++) {
    Text_PrintStringImmediate(9, 6 + i * 2, data_027c0280->layer0Map,
                              data_0229da8c[i], 0, 0);
  }
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
}
