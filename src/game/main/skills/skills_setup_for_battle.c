#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/battle_save.h"

#define STATE (data_027c027c->unk_48[data_027c0284])
/* The same state, reached through the screen table instead of data_027c027c. */
#define SCREEN_STATE                                       \
  (((Unk_02080440*)(&data_027c0288.unk_00)[data_027c0284]) \
       ->unk_48[data_027c0284])

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u32 gLinkPlay;
static u32 data_022a72b8[4];
/* skills a tag partner lends, per player and CO slot */
BattleSaveSkillSet data_022a72c8[5][2];
/* also declared in src/game/main/link/link_skill_sync_task_init.c and 5 more */
u32 gCoEquippedSkills[CO_COUNT][4][4];
extern u32 gBattleSkillSets[4][2][4];
/* Battle skill sets, indexed by player (1-4): gBattleSkillSets starts at player 1. */
#define PLAYER_SKILLS (gBattleSkillSets - 1)

void Skills_Copy(const u32* src, u32* dst);
const u32* LinkSkills_GetPlayerSet(int player, int set);
BOOL Campaign_IsHardMode(void);
int Co_GetForceRankSetIndex(int co, int player);
void Skills_SetFromList(const u8* list, u32* skills);

/* Fills the battle skill sets of players 1-4. frontSet and backSet pick
 * which of a player's two sets gets the front and the back CO's skills.
 * Every set is first reset from data_022a72b8; the Design Room stops there.
 * In link play each CO takes the set received over the link. Otherwise
 * human players take their CO's equipped Force Rank set (the one saved for
 * that player in Versus, set 0 elsewhere), and computer players take the
 * map's skill lists (Campaign only in hard mode; War Room and Survival
 * always) or, in Versus, the equipped set too. */
void Skills_SetupForBattle(int mode, int mapId, int frontSet, int backSet) {
  /* needed to match: each loop has its own counters and CO local */
  int humanCo;
  int linkPlayer;
  int player;
  int i;
  int versusI;
  int sets[2];
  int humanPlayer;
  int versusCo;
  int linkI;
  int humanI;

  sets[0] = frontSet;
  sets[1] = backSet;

  for (player = 1; player < 5; player++) {
    for (i = 0; i < 2; i++) {
      Skills_Copy(data_022a72b8, PLAYER_SKILLS[player][sets[i]]);
    }
  }
  if (STATE->gameMode == GAME_MODE_DESIGN_ROOM) {
    return;
  }
  if (gLinkPlay != 0) {
    for (linkPlayer = 1; linkPlayer < 5; linkPlayer++) {
      if (STATE->players[linkPlayer].controller != PLAYER_CONTROLLER_NONE) {
        for (linkI = 0; linkI < 2; linkI++) {
          if ((&STATE->players[linkPlayer].front)[linkI].id &
              PLAYER_CO_ID_MASK) {
            Skills_Copy(LinkSkills_GetPlayerSet(linkPlayer, sets[linkI]),
                        PLAYER_SKILLS[linkPlayer][sets[linkI]]);
          }
        }
      }
    }
    return;
  }
  for (humanPlayer = 1; humanPlayer < 5; humanPlayer++) {
    if (STATE->players[humanPlayer].controller != PLAYER_CONTROLLER_NONE &&
        SCREEN_STATE->players[humanPlayer].controller ==
            PLAYER_CONTROLLER_HUMAN) {
      for (humanI = 0; humanI < 2; humanI++) {
        humanCo =
            (&STATE->players[humanPlayer].front)[humanI].id & PLAYER_CO_ID_MASK;
        if (humanCo != 0) {
          Skills_Copy(gCoEquippedSkills[humanCo][mode == GAME_MODE_VERSUS
                                                     ? Co_GetForceRankSetIndex(
                                                           humanCo, humanPlayer)
                                                     : 0],
                      PLAYER_SKILLS[humanPlayer][sets[humanI]]);
        }
      }
    }
  }
  for (player = 1; player < 5; player++) {
    if (STATE->players[player].controller != PLAYER_CONTROLLER_NONE &&
        SCREEN_STATE->players[player].controller != PLAYER_CONTROLLER_HUMAN) {
      switch (mode) {
        case GAME_MODE_CAMPAIGN:
          for (i = 0; i < 2; i++) {
            Skills_SetFromList(
                Campaign_IsHardMode()
                    ? Ov000_gMapHeaders[mapId].skills[player][sets[i]]
                    : NULL,
                PLAYER_SKILLS[player][sets[i]]);
          }
          break;
        case GAME_MODE_WAR_ROOM:
        case GAME_MODE_SURVIVAL:
          for (i = 0; i < 2; i++) {
            Skills_SetFromList(Ov000_gMapHeaders[mapId].skills[player][sets[i]],
                               PLAYER_SKILLS[player][sets[i]]);
          }
          break;
        case GAME_MODE_VERSUS:
          for (versusI = 0; versusI < 2; versusI++) {
            versusCo =
                (&STATE->players[player].front)[versusI].id & PLAYER_CO_ID_MASK;
            if (versusCo != 0) {
              Skills_Copy(gCoEquippedSkills[versusCo][Co_GetForceRankSetIndex(
                              versusCo, player)],
                          PLAYER_SKILLS[player][sets[versusI]]);
            }
          }
          break;
      }
    }
  }
}
