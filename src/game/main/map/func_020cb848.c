#include "types.h"
#include "game/Unk_02080440.h"
#include "game/info_panel.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern s16 data_0216635c[INFO_PANEL_SIDES][3]; /* terrain box x */
extern s16 data_0216635e[INFO_PANEL_SIDES][3]; /* unit box x */

int Anim_GetPulseFrame(void);
int Map_GetHitTargetKind(int x, int y);
void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16* pos);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
void func_020d2df0(int x, int y, int value, int tile);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define UNIT_AT_CURSOR \
  (STATE->mapTiles     \
       ->unitAt[STATE->mapTiles->rowStart[STATE->cursorY] + STATE->cursorX])

/* Damage forecast over the info panel while the acting unit picks a target:
 * the damage it would deal from its destination tile to the unit under the
 * cursor (over the unit box), or to a map object or tile with hp (over the
 * terrain box). The number bobs with Anim_GetPulseFrame. */
void func_020cb848(int panelX, int panelY, int side) {
  int pulse = Anim_GetPulseFrame();
  u8 target = UNIT_AT_CURSOR;
  int kind = Map_GetHitTargetKind(STATE->cursorX, STATE->cursorY);

  if (target != 0) {
    Combat_SimulateChecked(STATE->actingUnitIndex, UNIT_AT_CURSOR,
                           &STATE->actingUnitDestX);
    func_020d2df0((u16)(panelX + data_0216635e[(s16)side][0] + 27),
                  (u16)(panelY - 20 - pulse),
                  (u16)STATE->combatants[0].damageNoLuck, 0x70);
  } else if (kind != 0) {
    Combat_CalcDamageToTarget(STATE->actingUnitIndex, kind,
                              &STATE->actingUnitDestX);
    func_020d2df0((u16)(panelX + data_0216635c[(s16)side][0] + 24),
                  (u16)(panelY - 20 - pulse),
                  (u16)STATE->combatants[0].damageNoLuck, 0x70);
  }
}
