#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/script_slot.h"

extern const ScriptCommand data_021666e8[];
extern int data_022a6dd4;
extern s16 data_021663dc
    []; /* 3 per side of the unit: bit for data_022a6dd4, tile x offset, tile y offset */
extern u32 gLinkPlay;

s8 ScriptSlot_Find(const ScriptCommand* script);
void ScriptSlot_Stop(int slot);
void ActingUnit_FinishAction(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020b9d48(void);
void MenuDepth_Pop(void);
BOOL Tile_IsOnScreen(int x, int y);
int Supply_ResupplyUnit(MapUnit* unit);
void ServiceCostPopup_Start(s16* pos, int player, int cost, u32 arg3, int arg4,
                            int arg5);

/* Script callback of the supply command. For each side of the acting unit
 * that Supply_FlagAdjacentTargets flagged, refills the unit there for free.
 * A unit that is on screen on a shown tile and got something starts the
 * service cost popup, and the next side waits until the popup is gone. After
 * the fourth side the script stops and the acting unit's action ends. */
void Supply_RefillAdjacentStep(SupplySlot* slot) {
  MapUnit* unit;
  int cost;
  s16 pos[2];

  if (ScriptSlot_Find(data_021666e8) != -1) {
    return;
  }
  for (;;) {
    if (slot->side >= 4) {
      if (slot->unk_20 == 0) {
        MAP_STATE->cursorX = MAP_STATE->actingUnitDestX;
        MAP_STATE->cursorY = MAP_STATE->actingUnitDestY;
      }
      ScriptSlot_Stop(*data_027c027c->scriptSlotCurrent);
      ActingUnit_FinishAction();
      if (slot->unk_20 == 0 && gLinkPlay != 0) {
        UnitAction_Send(6, MAP_STATE->actingUnitIndex, 0, 0);
      }
      func_020b9d48();
      MenuDepth_Pop();
      return;
    }
    if (data_022a6dd4 & data_021663dc[slot->side * 3]) {
      unit = &MAP_STATE->units[MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(
          MAP_STATE->actingUnitDestX + data_021663dc[slot->side * 3 + 1],
          MAP_STATE->actingUnitDestY + data_021663dc[slot->side * 3 + 2])]];
      if (MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(unit->x, unit->y)] ==
              0 ||
          !Tile_IsOnScreen(unit->x, unit->y)) {
        Supply_ResupplyUnit(unit);
      } else {
        cost = Supply_ResupplyUnit(unit);
        if (cost == 0) {
          slot->side++;
          continue;
        }
        pos[0] = MAP_STATE->actingUnitDestX + data_021663dc[slot->side * 3 + 1];
        pos[1] = MAP_STATE->actingUnitDestY + data_021663dc[slot->side * 3 + 2];
        ServiceCostPopup_Start(pos, (u8)MAP_STATE->currentPlayer, cost, 0, 0,
                               1);
        slot->side++;
        return;
      }
    }
    slot->side++;
  }
}
