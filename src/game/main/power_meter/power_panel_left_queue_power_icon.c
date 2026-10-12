#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

extern u8 data_02168680[];
extern u8 data_021686a0[];
extern u8 data_021686c0[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the power icon of a player at (x, y): one icon while a tag power
 * is on (both COs), otherwise one by the front CO's power state. Left-half
 * twin of PowerPanelRight_QueuePowerIcon; every icon sits 32 pixels right of x. */
void PowerPanelLeft_QueuePowerIcon(int x, int y, int player) {
  BOOL tagPowerOn = FALSE;

  if (STATE->players[player].back.id != 0) {
    BOOL isTagHalf = STATE->players[player].tagPowerHalf == TAG_POWER_FRONT ||
                     STATE->players[player].tagPowerHalf == TAG_POWER_BACK;

    if (isTagHalf) {
      tagPowerOn = TRUE;
    }
  }
  if (tagPowerOn) {
    Sprite_QueueAt(0, x + 32, y, data_021686a0, 0x7048, 0);
    return;
  }
  switch (STATE->players[player].front.powerState) {
    case 1:
    default:
      Sprite_QueueAt(0, x + 32, y, data_02168680, 0x7030, 0);
      break;
    case 2:
      Sprite_QueueAt(0, x + 32, y, data_021686c0, 0x703a, 0);
      break;
  }
}
