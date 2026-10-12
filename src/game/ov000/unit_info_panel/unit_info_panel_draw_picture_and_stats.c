#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"

void FS_ChangeDir(const char* dir);
u32 File_UnpackTo(void* dest, const char* path);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Font_LoadDigits(int baseTile);
int GetUnitMovement(int player, int unitType);
int Player_GetUnitVision(int player, int unitType);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);

extern Unk_027c0280* data_027c0280;
extern char data_ov000_0234f9d8[];
extern char data_ov000_0234f9e0[];
extern const char* data_ov000_0234f398[][15];
extern const char* data_ov000_0234f3ac[][15];

/* Redraws the picture block of the unit info panel at pixel x: swaps the panel's
 * OBJ picture buffer, loads the unit type's picture (by army style) and palette
 * (by army colour), clears a 15x20 tile block of layer 0, then prints the unit's
 * movement and vision. Oozium (UNIT_CLASS_ALL) also gets text 0xc00000af. The
 * row of 15 picture and palette file names per type is indexed by army. */
void UnitInfoPanel_DrawPictureAndStats(u32 x, MapUnit* unit, int palette,
                                       int style) {
  s16 row;
  s16 col;
  int base;
  int numberX;
  u16* map;

  data_ov000_0234ec8c->buffer = 1 - data_ov000_0234ec8c->buffer;
  FS_ChangeDir(data_ov000_0234f9d8);
  File_UnpackTo(data_027c0280->objVram +
                    (data_ov000_0234ec8c->buffer == 0 ? 532 : 596) * 32,
                data_ov000_0234f398[unit->type][style]);
  FS_ChangeDir(data_ov000_0234f9e0);
  Palette_LoadFromFile(data_ov000_0234f3ac[unit->type][palette], 0, 0x260,
                       0x40);
  base = x >> 3;
  for (row = 0; row < 20; row++) {
    for (col = 0; col < 15; col++) {
      data_027c0280->layer0Map[col + base] = 0;
    }
    base += 32;
  }
  Font_LoadDigits(256);
  map = data_027c0280->layer0Map;
  numberX = (int)(x + 112) >> 3;
  Number_Draw(numberX, 5, map,
              GetUnitMovement(data_ov000_0234ec8c->owner, unit->type), 0, 0);
  map = data_027c0280->layer0Map;
  Number_Draw(numberX, 7, map,
              Player_GetUnitVision(data_ov000_0234ec8c->owner, unit->type), 0,
              0);
  if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL) {
    Text_PrintImmediate((s16)((x + 104) >> 3), 9, data_027c0280->layer0Map,
                        0xc00000af, 0, 0);
  }
}
