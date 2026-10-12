#include "types.h"
#include "game/screen_other_map.h"
#include "game/player.h"

int MainLoop_GetFrameCount(void);
int CoPowerCost(int player, int slot);
int SuperPowerCost(int player, int slot);
int PowerStarCharge(int player, int slot);
void PowerMeter_QueueStars(int* x, int* y, int* charge, int starSize,
                           int length, int style, int bob, int* bobCounter,
                           int palette);

/* Draws the power meter of one CO: first the stars up to the CO Power cost
 * (style 0), then the stars up to the Super CO Power cost (style 1). The
 * stars bounce once their stretch is full. frontMeter reads the front CO's
 * charge, cost and star size instead of coSlot's, and does it with the
 * other screen's state set current. */
void PowerMeter_Draw(int x, int y, int player, int coSlot, int palette,
                     int frontMeter) {
  int charge;
  int coCost;
  int extraCost;
  int drawX;
  int starSize;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int drawY;
  int frame;

  frame = MainLoop_GetFrameCount();

  if (frontMeter != 0) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    savedScreen = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    charge = (&data_027c027c->unk_48[data_027c0284]->players[player].front)[0]
                 .charge;
    coCost = CoPowerCost(player, 0);
    extraCost = SuperPowerCost(player, 0) - coCost;
    starSize = PowerStarCharge(player, 0);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  } else {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
    savedScreen = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
    charge =
        (&data_027c027c->unk_48[data_027c0284]->players[player].front)[coSlot]
            .charge;
    coCost = CoPowerCost(player, coSlot);
    extraCost = SuperPowerCost(player, coSlot) - coCost;
    starSize = PowerStarCharge(player, coSlot);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }
  drawX = x + 24;
  drawY = y + 6;
  PowerMeter_QueueStars(&drawX, &drawY, &charge, starSize, coCost, 0,
                        charge >= coCost, &frame, palette);
  PowerMeter_QueueStars(&drawX, &drawY, &charge, starSize, extraCost, 1,
                        charge >= extraCost, &frame, palette);
}
