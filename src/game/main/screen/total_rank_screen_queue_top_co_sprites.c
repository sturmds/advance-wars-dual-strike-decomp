#include "types.h"
#include "game/campaign_map.h"
#include "game/screen_other_map.h"

CampaignTopCo* Campaign_GetUnlockedCos(void);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueCoSlotFrame(int a0, u16 x, u16 y, int a3);
void Co_QueueSmallPictureSprite(int x, int y, int attr, int list);
void Sprite_QueueDefault(int x, int y, int attr, int list);

extern u8 data_027c0060;

#define TOP_CO_COUNT 3 /* entries of Campaign_GetUnlockedCos's list */

/* Total rank screen, sub screen: one row per top CO from y 128, 16 pixels
 * apart. Each row gets a panel sprite and, when the slot holds a CO, the CO
 * slot frame, the CO's small picture and one more sprite. x shifts the whole
 * group (the slide-in passes its offset). */
void TotalRankScreen_QueueTopCoSprites(int x) {
  CampaignTopCo* top;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int i;
  int y;

  top = Campaign_GetUnlockedCos();
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  for (i = 0; i < TOP_CO_COUNT; i++, top++) {
    y = 128 + i * 16;
    Sprite_QueueAt(0, (x + 8) & 0x1ff, y & 0xff, &data_027c0060, i * 8, 0);
    if (top->co != 0) {
      Sprite_QueueCoSlotFrame(1, (x + 32) & 0x1ff, y & 0xff, 0);
      Co_QueueSmallPictureSprite((x + 56) & 0x1ff, (y + 16) & 0xff,
                                 (24 + i * 12) | ((i + 1) << 12), 2);
      Sprite_QueueDefault((x + 80) & 0x1ff, (y + 1) & 0xff,
                          (60 + i * 12) | 0x4000, 3);
    }
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
