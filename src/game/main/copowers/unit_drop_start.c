#include "types.h"
#include "game/UnitDropTask.h"
#include "game/Unk_02080440.h"

void PowerHighlight_LoadEffect(int player);

/* First step of gUnitDropScript (Sensei's powers): starts the search for
 * drop sites at tile (0, 0) and starts the highlight of the current
 * player's power (PowerHighlight_LoadEffect reads highlightEffect and highlightColor
 * of the front CO's block). */
void UnitDrop_Start(UnitDropTask* task) {
  task->x = 0;
  task->y = 0;
  PowerHighlight_LoadEffect(MAP_STATE->currentPlayer);
}
