#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Battle_ResetTurnState(void);
void BattleSave_Restore(int recordId);
void func_020c1c14(void);
void MapSelect_PointAtMap(int mapId);
void Screens_SelectConfigs(void);
void Battle_RegisterFrameCallbacks(void);
BOOL Map_IsCoSelectPhase(void);
void SetupCoSelection(void);
void func_020e88f8(void);
void Player_PlayTheme(int playerIndex);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Callback of the battle resume script data_02158468: restores the battle
 * save record in the slot, sets the map up again and plays the current
 * player's theme. */
void Battle_ResumeFromSave(BattleResumeSlot* slot) {
  Battle_ResetTurnState();
  BattleSave_Restore(slot->recordId);
  func_020c1c14();
  MapSelect_PointAtMap(STATE->unk_1c4->mapId);
  Screens_SelectConfigs();
  if (HOOKS->unk_54 != NULL) {
    HOOKS->unk_54(1);
  }
  Battle_RegisterFrameCallbacks();
  if (Map_IsCoSelectPhase()) {
    SetupCoSelection();
  }
  func_020e88f8();
  Player_PlayTheme(
      (u8)STATE
          ->currentPlayer); /* needed to match: the cast emits the and 0xff */
}
