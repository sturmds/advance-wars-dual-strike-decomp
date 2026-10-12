#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

void ActivatePower(int player, int power);
void func_020c4018(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Starts the super power of the current player's front CO. */
void ActivateCurrentSuperPower(void) {
  ActivatePower(data_027c027c->unk_48[data_027c0284]->currentPlayer,
                POWER_SUPER);
  func_020c4018();
}
