#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

void func_020dda74(void);
void func_020dda8c(int x, int y, int a2);
void func_0201e954(void);

/* Unit command menu handler (table entry at 0x021660e0). In the campaign mission
 * MAP_CAMPAIGN_FIRST + 18 it calls func_020dda8c with the acting unit's destination;
 * otherwise it calls func_020dda74. Both start a task; what they do is not known. */
void func_020bd358(void) {
  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN &&
      MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 18) {
    func_020dda8c(MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY, 3);
  } else {
    func_020dda74();
  }
  func_0201e954();
}
