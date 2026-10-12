#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229c734.h"
#include "game/Unk_02160abc.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
u8* File_Load(const char* path);
void Mem_Free(void* block);
int Co_GetNation(int co);
int CombatCo_IdFromNumber(int value);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const char data_0215a4cc[];

/* Part 4 of the result points screen: 20 tiles from /combat_st/res_cmmenu,
 * plus the army palettes of the two sides (data_02160abc colors 1-based) in
 * OBJ palettes paletteSlot + 3 and + 4. Also records both COs' nations. */
void ResultPoint_LoadArmyPart(void) {
  u8* file;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  u8 color;

  data_0229c734->partTile[4] = data_0229c734->nextTile;
  file = File_Load(data_0215a4cc);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Data_Unpack(file + 0x12cc,
              data_027c0280->objVram + data_0229c734->nextTile * 32);
  color = data_02160abc->unk_0d;
  if (color != 0) {
    Palette_Load(file + 0x16d4 + (color - 1) * 32,
                 (u16)((data_0229c734->paletteSlot + 19) * 32), 32);
  }
  color = data_02160abc->unk_0e;
  if (color != 0) {
    Palette_Load(file + 0x16d4 + (color - 1) * 32,
                 (u16)((data_0229c734->paletteSlot + 20) * 32), 32);
  }
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
  data_0229c734->nextTile += 20;
  Mem_Free(file);
  data_0229c734->nations[0] =
      Co_GetNation(CombatCo_IdFromNumber(data_02160abc->unk_0b));
  data_0229c734->nations[1] =
      Co_GetNation(CombatCo_IdFromNumber(data_02160abc->unk_0c));
}
