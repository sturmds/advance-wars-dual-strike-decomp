#include "types.h"

void Sprites_ResetLists(void);
void OamBuffer_Split(int headCount);

void func_020032a8(void) {
  Sprites_ResetLists();
  OamBuffer_Split(16);
}
