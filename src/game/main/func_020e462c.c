#include "types.h"
#include "game/Unk_02080440.h"

/* Adds offset `index` of offset list `list` to x and y; FALSE at the end of the list. */
BOOL Map_AddTileOffset(int* x, int* y, int list, int index);
void Unit_DamageAtTile(int x, int y, int arg);
void Unit_ParalyseAtTile(int x, int y, int player);
void func_020c4018(void);
void Power_ResetScrollAndRefresh(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Runs Unit_DamageAtTile and Unit_ParalyseAtTile for every tile of offset list 2 around (x, y). */
void func_020e462c(int x, int y) {
  int pos[2];
  int index = 0;

  do {
    pos[0] = x;
    pos[1] = y;
    if (!Map_AddTileOffset(&pos[0], &pos[1], 2, index++)) {
      break;
    }
    Unit_DamageAtTile(pos[0], pos[1], 30);
    Unit_ParalyseAtTile(pos[0], pos[1],
                        data_027c027c->unk_48[data_027c0284]->currentPlayer);
  } while (1);
  func_020c4018();
  Power_ResetScrollAndRefresh();
}
