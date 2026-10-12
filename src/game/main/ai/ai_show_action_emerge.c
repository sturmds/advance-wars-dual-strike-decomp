#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Unit_PlayEmergeEffect(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Display step of action 12: the acting unit ends its dive. */
void AiShowAction_Emerge(void) {
  Unit_PlayEmergeEffect(STATE->actingUnit->type);
  STATE->actingUnit->flags &= ~MAP_UNIT_DIVED;
}
