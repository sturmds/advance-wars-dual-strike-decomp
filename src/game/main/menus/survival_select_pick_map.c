#include "types.h"
#include "game/page_strip.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void MapPreview_Draw(void* dst, int mapId);
void PageStrip_AddButtons(void);
void Sound_PlaySe(u32 soundId);
void SurvivalSelect_DrawMapRecord(void);
void SurvivalSelect_LoadCoPictures(void);
int Survival_GetMapId(int mode, int index);
int SurvivalSelect_GetList(void);

extern Unk_022a7b80 data_022a7b80;

/* Button handler of the Survival map select: moves the current list's
 * cursor to the entry the button stands for (*entry), then redraws the
 * buttons, the record, the CO pictures and the map preview. */
void SurvivalSelect_PickMap(u8* entry) {
  int list = SurvivalSelect_GetList();
  Unk_027c0280* savedState;

  Sound_PlaySe(0x10002);
  data_0229c744->lists[list].cursor = *entry;
  PageStrip_AddButtons();
  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  SurvivalSelect_DrawMapRecord();
  SurvivalSelect_LoadCoPictures();
  MapPreview_Draw(
      data_027c0280->objVram + 0x8000,
      Survival_GetMapId(data_022a7b80.mode,
                        data_0229c744->lists[SurvivalSelect_GetList()].cursor));
  Screen_SetCurrentGfx(savedState);
}
