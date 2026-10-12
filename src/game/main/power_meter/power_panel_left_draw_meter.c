#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int MainLoop_GetFrameCount(void);
int CoPowerCost(int player, int slot);
int SuperPowerCost(int player, int slot);
int PowerStarCharge(int player, int slot);
void PowerPanelLeft_QueueStars(int* x, int* y, int* charge, int starSize,
                               int length, int style, int bob, int* bobCounter);

/* Draws the power meter of one CO (coSlot 0 front, 1 back) on the left
 * power panel: the CO Power stars, then the Super CO Power stars. A stretch
 * bounces once it is full. Like PowerMeter_Draw (ov000), without the
 * screen switch. */
void PowerPanelLeft_DrawMeter(int x, int y, int player, int coSlot) {
  int charge;
  int coCost;
  int extraCost;
  int starSize;
  int drawX;
  int drawY;
  int frame;

  charge =
      (&data_027c027c->unk_48[data_027c0284]->players[player].front)[coSlot]
          .charge;
  coCost = CoPowerCost(player, coSlot);
  extraCost = SuperPowerCost(player, coSlot) - coCost;
  starSize = PowerStarCharge(player, coSlot);
  frame = MainLoop_GetFrameCount();
  drawX = x + 32;
  drawY = y + 6;
  PowerPanelLeft_QueueStars(&drawX, &drawY, &charge, starSize, coCost, 0,
                            charge >= coCost, &frame);
  PowerPanelLeft_QueueStars(&drawX, &drawY, &charge, starSize, extraCost, 1,
                            charge >= extraCost, &frame);
}
