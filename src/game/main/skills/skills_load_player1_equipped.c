#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gBattleSkillSets[][PLAYER_CO_COUNT][SKILL_SET_WORDS];

void Skills_Copy(const u32* src, u32* dst);

/* Copies the equipped skill set 0 of player 1's front and back CO into
 * that player's battle sets frontSet and backSet. */
void Skills_LoadPlayer1Equipped(int frontSet, int backSet) {
  int sets[PLAYER_CO_COUNT];
  int i;

  sets[0] = frontSet;
  sets[1] = backSet;
  for (i = 0; i < PLAYER_CO_COUNT; i++) {
    int co = (&data_027c027c->unk_48[data_027c0284]->players[1].front)[i].id &
             PLAYER_CO_ID_MASK;

    if (co != 0) {
      Skills_Copy(gCoEquippedSkills[co][0], gBattleSkillSets[0][sets[i]]);
    }
  }
}
