#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229c734.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Data_Unpack(const void* src, void* dest);
void ResultPoint_LoadArmyPart(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define PART_DEST (data_027c0280->objVram + data_0229c734->nextTile * 32)

/* Unpacks graphics part 0-4 of the result points screen into OBJ VRAM of
 * the main screen at nextTile and records where it starts. Parts 0-3 come
 * from gfxFile; part 4 (ResultPoint_LoadArmyPart) loads its own file. */
void ResultPoint_LoadPart(int part) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_0229c734->partTile[part] = data_0229c734->nextTile;
  switch (part) {
    case 0:
      Data_Unpack(data_0229c734->gfxFile + 0x39c, PART_DEST);
      data_0229c734->nextTile += 60;
      break;
    case 1:
      Data_Unpack(data_0229c734->gfxFile, PART_DEST);
      data_0229c734->nextTile += 20;
      break;
    case 2:
      Data_Unpack(data_0229c734->gfxFile + 0x580, PART_DEST);
      data_0229c734->nextTile += 48;
      break;
    case 3:
      Data_Unpack(data_0229c734->gfxFile + 0xfc, PART_DEST);
      data_0229c734->nextTile += 80;
      break;
    case 4:
      ResultPoint_LoadArmyPart();
      break;
  }
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
}
