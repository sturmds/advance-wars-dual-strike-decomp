#include "types.h"
#include "game/player.h"
#include "game/map_header.h"
#include "game/page_strip.h"
#include "game/survival_record.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

void Co_QueueSmallPicture(int coId, void* dest, int paletteSlot);
void Mem_Zero32(u32 value, void* dst, u32 size);
u8 Map_GetPlayerCount(u16 mapId);
BOOL SurvivalSelect_HasRecord(int index);
int SurvivalSelect_GetList(void);
int Survival_GetMapId(int mode, int index);
int Survival_GetRecordIndex(int mode, int index);

extern Unk_022a7b80 data_022a7b80;

/* Record COs of the map at the strip cursor, for the current mode. */
#define RECORD(cursor) \
  (data_02291588[Survival_GetRecordIndex(data_022a7b80.mode, cursor)])
#define RECORD1(cursor) \
  (data_0229158c[Survival_GetRecordIndex(data_022a7b80.mode, cursor)])

/* Loads the 12-tile picture of CO coId into OBJ VRAM at byte offset
 * offset, or clears those tiles when coId is 0. */
#define LOAD_CO(coId, offset, palette)                                    \
  co = coId;                                                              \
  if (co != 0) {                                                          \
    Co_QueueSmallPicture(co, data_027c0280->objVram + (offset), palette); \
  } else {                                                                \
    Mem_Zero32(0, data_027c0280->objVram + (offset), 0x180);              \
  }

/* Survival map select: loads the CO pictures for the map at the strip
 * cursor. When SurvivalSelect_HasRecord reports a record for the current mode, its
 * two COs go to palettes 22 and 23; otherwise those tiles are cleared.
 * Then the map's own COs for slots 2 up to the player count (the
 * opponents), two per slot, go to tiles 0, 12, 24, ... with palettes from
 * 16. */
void SurvivalSelect_LoadCoPictures(void) {
  int i;
  int cursor = data_0229c744->lists[SurvivalSelect_GetList()].cursor;
  int playerCount =
      Map_GetPlayerCount(Survival_GetMapId(data_022a7b80.mode, cursor));
  int j;
  int co;

  if (SurvivalSelect_HasRecord(cursor)) {
    switch (data_022a7b80.mode) {
      case 0:
      case 3:
        LOAD_CO(RECORD(cursor).mode0Co, 0x900, 22);
        LOAD_CO(RECORD(cursor).mode0TagCo, 0xa80, 23);
        break;
      case 2:
      case 5:
        LOAD_CO(RECORD(cursor).mode2Co, 0x900, 22);
        LOAD_CO(RECORD(cursor).mode2TagCo, 0xa80, 23);
        break;
      default:
        LOAD_CO(RECORD1(cursor).mode1Co, 0x900, 22);
        LOAD_CO(RECORD1(cursor).mode1TagCo, 0xa80, 23);
        break;
    }
  } else {
    Mem_Zero32(0, data_027c0280->objVram + 0x900, 0x300);
  }
  for (i = 0; i < playerCount - 1; i++) {
    /* Needed to match: the bound is written as <= count - 1. */
    for (j = 0; j <= PLAYER_CO_COUNT - 1; j++) {
      co = Ov000_gMapHeaders[Survival_GetMapId(data_022a7b80.mode, cursor)]
               .co[i + 2][j];
      if (co != 0) {
        Co_QueueSmallPicture(co,
                             data_027c0280->objVram + (((j + i * 2) * 12) << 5),
                             j + i * 2 + 16);
      } else {
        Mem_Zero32(0, data_027c0280->objVram + (((j + i * 2) * 12) << 5),
                   0x180);
      }
    }
  }
}
