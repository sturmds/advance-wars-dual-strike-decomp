#include "types.h"
#include "game/Unk_0229c734.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_022a7a9c.h"

/* Returns 1, 2 or 3 from data_02160ac0->unk_84 (0, 1, 2). */
int ScoreList_GetMultiplier(void);
/* Counts the cells with state 1 and this tile in data_02160ac0, at most 9. */
u8 CombatCells_CountTile(int tile);

#define ROW (data_0229c734->rows[data_0229c734->rowCount])

/* Appends the score row for `kind` (1-21) to data_0229c734's list and returns
 * TRUE. Kinds 4-12 add nothing and return FALSE when their count is 0, as does
 * any kind outside 1-21. Kinds 9
 * and 13-20 cap the points at 999. */
BOOL ScoreList_AddRow(int kind) {
  u8 count;
  int points;

  switch (kind) {
    case 1:
      ROW.kind = kind;
      ROW.count = 0;
      ROW.points = data_022a7a9c.speed;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 2:
      ROW.kind = kind;
      ROW.count = 0;
      ROW.points = data_022a7a9c.power;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 3:
      ROW.kind = kind;
      ROW.count = 0;
      ROW.points = data_022a7a9c.technique;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 21:
      ROW.kind = kind;
      ROW.count = 0;
      ROW.points = ScoreList_GetMultiplier() * 300;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 4:
      count = data_022a7a9c.unk_0d;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = data_022a7a9c.unk_0d * 30;
      ROW.height = 32;
      data_0229c734->rowCount++;
      return TRUE;
    case 5:
      count = data_022a7a9c.unk_08;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = data_022a7a9c.unk_08 * 30;
      ROW.height = 32;
      data_0229c734->rowCount++;
      return TRUE;
    case 6:
      count = data_022a7a9c.unk_0a;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = data_022a7a9c.unk_0a * 20;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 7:
      count = data_022a7a9c.unk_09;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = data_022a7a9c.unk_09 * 10;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 8:
      count = data_022a7a9c.unk_0b;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = data_022a7a9c.unk_0b * 5;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 9:
      count = data_022a7a9c.unk_0c;
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      points = data_022a7a9c.unk_0c * 40;
      if (points >= 999) {
        points = 999;
      }
      ROW.points = points;
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 10:
      count = data_02160abc->unk_00[7];
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 40 * ScoreList_GetMultiplier();
      ROW.height = 32;
      data_0229c734->rowCount++;
      return TRUE;
    case 11:
      count = data_02160abc->unk_00[6];
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 10 * ScoreList_GetMultiplier();
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 12:
      count = data_02160abc->unk_00[9];
      if (count == 0) {
        break;
      }
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 5 * ScoreList_GetMultiplier();
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 17:
      count = data_02160abc->unk_00[1];
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 5 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 18:
      count = data_02160abc->unk_00[3];
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 10 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 19:
      count = data_02160abc->unk_00[4];
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 15 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 20:
      count = data_02160abc->unk_00[5];
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 20 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 13:
      count = CombatCells_CountTile(1);
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 25 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 14:
      count = CombatCells_CountTile(3);
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 50 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 15:
      count = CombatCells_CountTile(4);
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 75 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
    case 16:
      count = CombatCells_CountTile(5);
      ROW.kind = kind;
      ROW.count = count;
      ROW.points = count * 100 * ScoreList_GetMultiplier();
      if (ROW.points > 999) {
        ROW.points = 999;
      }
      ROW.height = 16;
      data_0229c734->rowCount++;
      return TRUE;
  }
  return FALSE;
}
