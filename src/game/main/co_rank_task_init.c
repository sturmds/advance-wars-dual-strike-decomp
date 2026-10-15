#include "types.h"
#include "game/co_rank_task.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"
#include "game/math.h"

int Co_GetRank(u32 co);
u32 Co_GetExperience(u32 co);
void Gfx_RequestLayerCopy(int layer);
void Gfx_ClearLayerMap(int layer);
void func_0201e6e8(int x, int y, int width, int height);

/* Sets up layers 0 and 1 for the rank screen (layer 1 scrolled 256
 * pixels right), fills an 8 by 5 tile area of layer 1 per CO shown
 * (func_0201e6e8), and reads player 1's
 * front CO (and back CO in a tag battle) rank and experience toward the
 * next rank into the task. */
void CoRankTask_Init(CoRankTask* task) {
  u8 layer1;
  u8 layer0;

  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  layer1 = data_027c0280->layer1Bg;
  layer0 = data_027c0280->layer0Bg;
  data_027c0280->bgScrollY[layer1] = 0;
  data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
  Screen_SetLayerScreenSize(1, 1);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0x100;
  func_0201e6e8(4, 17, 8, 5);
  if (task->hasBackCo == 1) {
    func_0201e6e8(21, 17, 8, 5);
  }
  task->cos[0].rank =
      Co_GetRank(MAP_STATE->players[1].front.id & PLAYER_CO_ID_MASK);
  task->cos[0].exp = HwMod32(
      Co_GetExperience(MAP_STATE->players[1].front.id & PLAYER_CO_ID_MASK),
      1000);
  task->cos[0].frame = 0;
  if (task->hasBackCo == 1) {
    task->cos[1].rank =
        Co_GetRank(MAP_STATE->players[1].back.id & PLAYER_CO_ID_MASK);
    task->cos[1].exp = HwMod32(
        Co_GetExperience(MAP_STATE->players[1].back.id & PLAYER_CO_ID_MASK),
        1000);
  } else {
    task->cos[1].rank = 0;
    task->cos[1].exp = 0;
  }
  task->cos[1].frame = 0;
}
