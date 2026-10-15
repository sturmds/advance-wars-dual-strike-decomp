#include "types.h"
#include "game/page_strip.h"
#include "game/survival_record.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"
#include "game/text_print.h"

void Bg_ClearRect(u16* map, int x, int y, int width, u16 height);
void Gfx_RequestLayerCopy(int bg);
void Layer0_PutNumberThenText3(int x, int y, u32 value);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
const void* Map_GetName(u16 mapId);
int Survival_GetMapId(int mode, int index);
BOOL SurvivalSelect_HasRecord(int index);
int SurvivalSelect_GetList(void);
int Survival_GetRecordIndex(int mode, int index);

extern Unk_022a7b80 data_022a7b80;

#define TEXT_NO_RECORD 0xc00000af

/* Survival map select, layer 0: the name of the map at the strip cursor and
 * its saved record for the current mode (data_022a7b80.mode): best time as minutes and
 * seconds (0, 3), fewest days (2, 5) or least funds spent (1, 4). Text
 * 0xc00000af stands in for an empty list or a map with no record
 * (SurvivalSelect_HasRecord). Needed to match: map read into a local before the name
 * lookup. */
void SurvivalSelect_DrawMapRecord(void) {
  PageStripList* list = &data_0229c744->lists[SurvivalSelect_GetList()];
  int cursor = list->cursor;
  int mode;
  u16* map;

  Bg_ClearRect(data_027c0280->layer0Map, 12, 18, 16, 6);
  if (list->count == 0) {
    Text_PrintId(13, 19, data_027c0280->layer0Map, TEXT_NO_RECORD, 0, 0x300);
    Text_PrintId(19, 21, data_027c0280->layer0Map, TEXT_NO_RECORD, 0, 0x380);
  } else {
    map = data_027c0280->layer0Map;
    Text_Print(13, 19, map,
               Map_GetName(Survival_GetMapId(data_022a7b80.mode, cursor)), 0,
               0x300);
    if (SurvivalSelect_HasRecord(list->cursor)) {
      mode = data_022a7b80.mode;
      switch (mode) {
        case 0:
        case 3:
          Layer0_PutRatio(
              20, 21,
              HwDiv32(data_02291650[Survival_GetRecordIndex(mode, cursor)], 60),
              HwMod32(data_02291650[Survival_GetRecordIndex(mode, cursor)],
                      60));
          break;
        case 2:
        case 5:
          Layer0_PutNumberThenText3(
              20, 21,
              data_02291588[Survival_GetRecordIndex(mode, cursor)].days);
          break;
        default:
          Layer0_PutNumberThenText4(
              20, 21,
              data_02291584[Survival_GetRecordIndex(mode, cursor)].fundsSpent);
          break;
      }
    } else {
      Text_PrintId(19, 21, data_027c0280->layer0Map, TEXT_NO_RECORD, 0, 0x380);
    }
  }
  Gfx_RequestLayerCopy(0);
}
