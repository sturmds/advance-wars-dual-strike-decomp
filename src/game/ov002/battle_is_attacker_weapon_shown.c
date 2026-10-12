#include "types.h"
#include "game/battle_scene.h"
#include "game/BattleSideData.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

#define MENU ((MenuState*)&gMenuState)

/* TRUE for the attacking side when its animation data does not have unk_48 set to 1 and the
 * entry for the weapon it fires is not 0 or 3. FALSE for the defender. */
BOOL Battle_IsAttackerWeaponShown(int side) {
  BattleSideData* data;
  int kind;

  if (side != Ov002_gBattleScene->attackerSide) {
    return FALSE;
  }
  data = MENU->battle.unk_27c[side];
  if (data->unk_48 == 1) {
    return FALSE;
  }
  kind = data->unk_49[Ov002_gBattleScene->sides[side].usesSecondary];
  if (kind == 3) {
    return FALSE;
  }
  return kind != 0;
}
