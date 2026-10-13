#include "types.h"
#include "game/Unk_02080440.h"

/* Needed to match: the original declares data_022a7a9c with its rows, so mwcc
 * folds the row offsets into the relocation. The header declares it as one
 * 0x28-byte entry; hide that declaration and use BattleResults here. */
#define data_022a7a9c data_022a7a9c_entry
#include "game/Unk_022a7a9c.h"
#undef data_022a7a9c
#include "game/terrain.h"

u8 data_022a7ac4[0x14]; /* size: up to the next symbol */
/* count: as many elements as fit before the next symbol */
u8 data_022a7ad8[0x4][40];

int Player_GetCoByteFallback(int player, int slot);
int func_02001730(
    int player, int slot); /* CO id in a player's front (0) or back (1) slot */
int Campaign_IsHardMode(void);
int Co_GetExperience(int co);
void Co_SetExperience(int co, int experience);
int ExperienceToRank(int experience);
s32 HwMod32(s32 numerator, s32 denominator);
int ExpBonus_Scale(int points,
                   int bonus); /* points times the EXP_BONUS_* multiplier */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern BattleResults data_022a7a9c;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define ROWS (data_022a7a9c.rows)

/* Fills the result row of each player slot at the end of a battle: CO ids,
 * army colour, and for each CO of a human player the experience change.
 * The multiplier comes from the game mode: x1 by default; in the
 * campaign x2 for a lone CO except on maps 224-231 on normal mode; in
 * the War Room by unk_59 (0: x2.5, 1: x2, 2: x1.5, 3: x1). Each CO first
 * loses the scaled expTaken (unless it has 100000 or more), then the row
 * keeps its ranks before and after the scaled expGain. Last, counts the
 * properties and units each player has on the map. */
void BattleResults_Fill(void) {
  /* declaration order and the u8 types needed to match */
  MapUnit* unit;
  int y;
  int co;
  u8 tile;
  int rankAfter;
  int taken;
  u8 bonus;
  u8 unitSlot;
  int i;
  int x;
  int player;
  int slot;
  int rankBefore;
  int gain;

  bonus = EXP_BONUS_X1;

  for (player = 1; player < 5; player++) {
    for (slot = 0; slot < 2; slot++) {
      co = func_02001730(player, slot);
      /* unk_00 and unk_02: front and back CO */
      (&ROWS[player].unk_00)[slot] = Player_GetCoByteFallback(player, slot);
      ROWS[player].armyColour = GAME->players[player].armyColour;
      switch (GAME->gameMode) {
        case GAME_MODE_CAMPAIGN:
          if (!Campaign_IsHardMode() && GAME->unk_1c4->mapId >= 224 &&
              GAME->unk_1c4->mapId <= 231) {
            bonus = EXP_BONUS_X1;
          } else if (slot == 0 && func_02001730(player, 1) == 0) {
            bonus = EXP_BONUS_X2;
          } else {
            bonus = EXP_BONUS_X1;
          }
          break;
        case GAME_MODE_WAR_ROOM:
          switch (GAME->unk_59) {
            case 0:
              bonus = EXP_BONUS_X2_5;
              break;
            case 1:
              bonus = EXP_BONUS_X2;
              break;
            case 2:
              bonus = EXP_BONUS_X1_5;
              break;
            case 3:
              bonus = EXP_BONUS_X1;
              break;
          }
          break;
        case GAME_MODE_SURVIVAL:
          bonus = EXP_BONUS_X1;
          break;
      }
      ROWS[player].expBonus[slot] = bonus;
      if (co != 0 &&
          GAME->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
        taken =
            ExpBonus_Scale(data_022a7a9c.expTaken, ROWS[player].expBonus[slot]);
        if (Co_GetExperience(co) < 100000) {
          if (Co_GetExperience(co) >= taken) {
            Co_SetExperience(co, Co_GetExperience(co) - taken);
          } else {
            Co_SetExperience(co, 0);
          }
        }
        gain =
            ExpBonus_Scale(data_022a7a9c.expGain, ROWS[player].expBonus[slot]);
        rankBefore = ExperienceToRank(Co_GetExperience(co));
        rankAfter = ExperienceToRank(gain + Co_GetExperience(co));
        ROWS[player].expInRank[slot] = HwMod32(Co_GetExperience(co), 1000);
        ROWS[player].expGain[slot] = gain;
        ROWS[player].rankBefore[slot] = rankBefore;
        ROWS[player].rankAfter[slot] = rankAfter;
      } else {
        ROWS[player].expInRank[slot] = 0;
        ROWS[player].expGain[slot] = 0;
        ROWS[player].rankBefore[slot] = 0;
        ROWS[player].rankAfter[slot] = 0;
      }
    }
    for (i = 0; i < 5; i++) {
      ROWS[player].propertyCounts[i] = 0;
    }
    ROWS[player].unitCount = 0;
  }

  for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
    for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
      unitSlot = GAME->mapTiles->unk_508[GAME->mapTiles->rowStart[y] + x];
      if (unitSlot != 0) {
        ROWS[(unitSlot >> 6) + 1].unitCount++;
        unit = &GAME->units[unitSlot];
        if (unit->cargo[0] != 0) {
          ROWS[(unitSlot >> 6) + 1].unitCount++;
        }
        if (unit->cargo[1] != 0) {
          ROWS[(unitSlot >> 6) + 1].unitCount++;
        }
      }
      tile = GAME->mapTiles->tiles[GAME->mapTiles->rowStart[y] + x];
      switch (MAP_TILE_TERRAIN(tile)) {
        case TERRAIN_CITY:
          ROWS[tile >> 5].propertyCounts[0]++;
          break;
        case TERRAIN_BASE:
          ROWS[tile >> 5].propertyCounts[1]++;
          break;
        case TERRAIN_AIRPORT:
          ROWS[tile >> 5].propertyCounts[2]++;
          break;
        case TERRAIN_PORT:
          ROWS[tile >> 5].propertyCounts[3]++;
          break;
        case TERRAIN_COM_TOWER:
          ROWS[tile >> 5].propertyCounts[4]++;
          break;
      }
    }
  }
}
