#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void func_020c4018(void);
int Ov006_RemoveUnitAt(int withEffect, int x, int y);
void func_ov006_0235f690(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Makes ov006 state unitArmy the current player, removes the unit on the tile
 * (cursorX, cursorY) with the effect, and returns 2 if there was one, else 0.
 * The meaning of the return value is a guess. */
int Ov006_DeleteMarkedUnit(void) {
  int result;

  STATE->currentPlayer = data_ov006_0236d640->unitArmy;
  STATE->currentUnitBase = (STATE->currentPlayer - 1) << 6;
  result = 0;
  if (Ov006_RemoveUnitAt(1, data_ov006_0236d640->cursorX,
                         data_ov006_0236d640->cursorY)) {
    func_020c4018();
    result = 2;
  }
  if (result > 0) {
    func_ov006_0235f690();
    data_ov006_0236d640->recountDelay = 5;
  }
  return result;
}
