#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Unit_PlayHideEffect(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Display step of action 11: the acting unit dives. */
void AiShowAction_Dive(void) {
  Unit_PlayHideEffect(STATE->actingUnit->type);
  STATE->actingUnit->flags |= MAP_UNIT_DIVED;
}
