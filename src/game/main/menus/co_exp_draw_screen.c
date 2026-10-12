#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_exp.h"
#include "game/win_lose.h"

int Campaign_IsHardMode(void);
int CoExp_IsSingleColumn(void);
void CoExp_DrawPlayerPanel(CoExp* exp, int x, int y, int player, int frame);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void CoExp_DrawCoCell(CoExp* exp, int x, int y, int coSlot, int frame);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_027c0048[];
extern u8 data_027c0060[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Queues the sprites of the CO experience page, offset by (x, y) so the
 * page can slide. Uses the layout CoExp_InitScreen drew: in the War Room
 * one centered CO cell (CoExp_IsSingleColumn) or player 0's panel, plus
 * the experience multiplier icon (tile 836); a single centered cell on
 * normal campaign maps up to 231; else one panel per side A player. */
void CoExp_DrawScreen(CoExp* exp, int x, int y, int frame) {
  if (GAME->gameMode == GAME_MODE_WAR_ROOM) {
    if (CoExp_IsSingleColumn() == 1) {
      int bannerX = (x + 112) & 0x1ff;

      Sprite_QueueAt(7, bannerX, (y + 42) & 0xff, data_027c0060, 0xb000, 0x2f4);
      CoExp_DrawCoCell(exp, x + 80, y + 56, 0, frame);
      Sprite_QueueAt(6, bannerX, (y + 104) & 0xff, data_027c0048, 0xc000,
                     0x344);
    } else {
      CoExp_DrawPlayerPanel(exp, x, y + 40, 0, frame);
      Sprite_QueueAt(6, (x + 112) & 0x1ff, (y + 104) & 0xff, data_027c0048,
                     0xc000, 0x344);
    }
  } else if (GAME->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode() &&
             GAME->unk_1c4->mapId <= 231) {
    Sprite_QueueAt(7, (x + 112) & 0x1ff, (y + 58) & 0xff, data_027c0060, 0xb000,
                   0x2f4);
    CoExp_DrawCoCell(exp, x + 80, y + 72, 0, frame);
  } else {
    switch (data_022a7a88.countA) {
      case 1:
        CoExp_DrawPlayerPanel(exp, x, y + 56, 0, frame);
        break;
      case 2:
        CoExp_DrawPlayerPanel(exp, x, y + 28, 0, frame);
        CoExp_DrawPlayerPanel(exp, x, y + 84, 1, frame);
        break;
      default:
        CoExp_DrawPlayerPanel(exp, x, y, 0, frame);
        CoExp_DrawPlayerPanel(exp, x, y + 56, 1, frame);
        CoExp_DrawPlayerPanel(exp, x, y + 112, 2, frame);
        break;
    }
  }
}
