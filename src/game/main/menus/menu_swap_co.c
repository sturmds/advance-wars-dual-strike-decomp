#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/unit_action.h"

extern UnitAction data_0229d8c8;
extern u32 gLinkPlay;

void func_0201e954(void);
void func_020ddb38(void);
void Link_SendWeather(int nextWeather);
void UnitAction_SwapCo(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Menu handler (table entries at 0x02165cc0 and 0x02165ce0): swaps the front
 * and back CO, sending unit action 15 first when gLinkPlay is set. With no
 * tag power on, func_020ddb38 then runs as in Menu_PickNextWeather: the swap
 * uses up the turn. */
void Menu_SwapCo(void) {
  func_0201e954();
  data_0229d8c8.rngState = gRngState;
  if (gLinkPlay != 0) {
    UnitAction_Send(15, 0, MAP_STATE->currentPlayer, 0);
  }
  UnitAction_SwapCo();
  switch (MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf) {
    case 0:
      func_020ddb38();
      if (gLinkPlay != 0) {
        Link_SendWeather(MAP_STATE->unk_1c4->nextWeather);
      }
      break;
    case TAG_POWER_FRONT: /* empty cases: needed to match the compares */
      break;
    case TAG_POWER_BACK:
      break;
  }
}
