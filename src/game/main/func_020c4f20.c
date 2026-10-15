#include "types.h"
#include "game/ai/ai_turn_setup.h"

void func_020c4448(void);
void Map_CountPlayerProperties(void);

void func_020c4f20(void) {
  func_020c4448();
  Map_CountPlayerProperties();
  AiTurn_FindUnitDomains();
}
