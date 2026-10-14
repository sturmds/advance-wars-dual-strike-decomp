#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/unlocks.h"
#include "game/Unk_ov005_0236bbb0.h"

void func_02101ac8(u32 id, int value);
void Map_SetSingleFlag(u32 mapId, int value);
void func_021019a4(u32 mapId, int value);

extern ShopItem data_0216cb60[];

/* New-game unlock state (Save_ResetProgress): first sets every map flag,
 * CO unlock, CO colour and the per-map-case flags of func_02101ac8, and
 * clears game flags GAME_FLAG_EARNED_FIRST to GAME_FLAG_CAMPAIGN_END - 1 and func_021019a4's per-map flags. Then each Battle Maps
 * shop item clears its own flag through its onBuy routine, so only what
 * the shop sells starts locked. */
void Unlocks_ResetToNewGame(void) {
  int i;

  for (i = 0; i < MAP_CASE_COUNT; i++) {
    func_02101ac8(i, 1);
  }
  for (i = 0; i < MAP_COUNT; i++) {
    Map_SetSingleFlag(i, 1);
  }
  for (i = 0; i < CO_COUNT; i++) {
    Unlock_Set(i, 1);
  }
  for (i = 0; i < CO_COUNT; i++) {
    CoColor_Set(i, 1);
  }
  for (i = GAME_FLAG_CAMPAIGN_FIRST; i < GAME_FLAG_CAMPAIGN_END; i++) {
    GameFlag_Set(i, 0);
  }
  for (i = GAME_FLAG_EARNED_FIRST; i < GAME_FLAG_CAMPAIGN_FIRST; i++) {
    GameFlag_Set(i, 0);
  }
  for (i = 0; i < MAP_COUNT; i++) {
    func_021019a4(i, 0);
  }
  for (i = 0; data_0216cb60[i].price != -1; i++) {
    data_0216cb60[i].onBuy(data_0216cb60[i].arg, 0);
  }
}
