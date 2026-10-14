#include "types.h"
#include "game/design_map.h"
#include "game/screen_other_map.h"
#include "game/save_progress.h"

extern SaveRecordBuffer data_02291774;

int func_0201fe28(u8 recordId);
int Save_LoadRecord(u8 recordId, void* buffer);
void Map_SetDataBuffer(void* buffer);
void SavedArmyColours_LoadFromRecord(DesignMapRecord* record);
void MapPreview_DrawToLayer(void* vram, u16 firstTile, int layer, int x, int y,
                            int palette);

/* Loads save record 7 + slot and draws its map preview at tile (x, y) of
 * layer 0, like DesignMap_DrawTemplatePreview. Returns 0 when the record is
 * unusable (nothing drawn), 1 otherwise. */
int DesignMap_DrawSlotPreview(int x, int y, int firstTile, int palette,
                              u8 slot) {
  DesignMapRecord* record = &data_02291774.designMaps[0];
  int recordId = slot + 7;

  if (func_0201fe28(recordId)) {
    return 0;
  }
  Save_LoadRecord(recordId, record);
  Map_SetDataBuffer(record);
  SavedArmyColours_LoadFromRecord(record);
  MapPreview_DrawToLayer(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      firstTile, 0, x, y, palette);
  return 1;
}
