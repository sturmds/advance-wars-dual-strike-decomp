#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"
#include "game/unit_stats.h"

extern MenuState gMenuState;
/* Tables of 20 bytes: five halfwords, then ten bytes counting up in fives. How
 * the HP drain uses them is not known yet. */
extern const u8 gBattleDrainProfile0[20];
extern const u8 gBattleDrainProfile1[20];
extern const u8 gBattleDrainProfile2[20];
extern const u8 gBattleDrainProfile3[20];
extern const u8 gBattleDrainProfile4[20];

/* Needed to match: through the cast the compiler keeps field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* A side's 12 bytes of the scene as BATTLE_SIDE_UNIT_TYPE sees them, from the start of the scene. */
typedef struct SideRecord {
  u8 pad_00[3];
  u8 unitType;
  u8 pad_04[5];
  u8 unk_09;
  u8 pad_0a[2];
} SideRecord;

#define RECORD(side) (((SideRecord*)Ov002_gBattleScene)[side])

/* Picks the drain profile of the attacker (index 0) and of the defender. A Sub
 * or type 32 (not in Ov000_gUnitStats) uses profile 4; the others pick one by
 * the kind from the side's data. */
void Battle_PickHpDrainProfiles(void) {
  int i;
  int side;
  int kind;

  for (i = 0; i < 2; i++) {
    side = i == 0 ? Ov002_gBattleScene->attackerSide
                  : (Ov002_gBattleScene->attackerSide + 1) & 1;
    if (RECORD(side).unitType == UNIT_SUB || RECORD(side).unitType == 32) {
      MENU->battle.drainProfile[i] = gBattleDrainProfile4;
    } else {
      kind = MENU->battle.unk_27c[side]->unk_49[RECORD(side).unk_09];
      if (kind == 3) {
        MENU->battle.drainProfile[i] = gBattleDrainProfile2;
      } else if (kind == 2) {
        MENU->battle.drainProfile[i] = gBattleDrainProfile1;
      } else if (MENU->battle.unk_48[side] != 0) {
        MENU->battle.drainProfile[i] = gBattleDrainProfile3;
      } else {
        MENU->battle.drainProfile[i] = gBattleDrainProfile0;
      }
    }
  }
}
