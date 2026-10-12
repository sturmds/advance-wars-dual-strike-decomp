#include "types.h"
#include "game/design_map.h"

extern DesignMap data_022a6c84[];

void String_Copy(char* dest, const char* src);

/* Copies the name of a player-made map into dest. Returns 0 for an unused row. */
int DesignMap_CopyName(u8 index, char* dest) {
  if (data_022a6c84[index].playerCount == 0xff) {
    return 0;
  }
  String_Copy(dest, data_022a6c84[index].name);
  return 1;
}
