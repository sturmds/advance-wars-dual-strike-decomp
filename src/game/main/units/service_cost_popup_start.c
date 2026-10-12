#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const ScriptCommand data_021666e8[];

ServiceCostPopupSlot* ScriptSlot_Start(const ScriptCommand* script, int arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starts the service cost popup script (data_021666e8) at tile pos[0], pos[1]
 * with the cost of the service and the current player's funds after paying it.
 * The player argument is not used. */
void ServiceCostPopup_Start(s16* pos, int player, int cost, u32 chargeFunds,
                            int zoomVariant, int visible) {
  ServiceCostPopupSlot* slot = ScriptSlot_Start(data_021666e8, 0);

  slot->cost = cost;
  slot->fundsAfter = STATE->players[STATE->currentPlayer].funds - cost;
  slot->tileX = pos[0];
  slot->tileY = pos[1];
  slot->chargeFunds = chargeFunds;
  slot->zoomVariant = zoomVariant;
  slot->visible = visible;
}
