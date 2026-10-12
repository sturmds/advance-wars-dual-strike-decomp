#include "types.h"
#include "game/design_map.h"

extern DesignMap data_022a6c84[];
extern const char data_021674b4[];

/* Returns the name of a player-made map, or a default name for an unused row. */
const char* DesignMap_GetName(u8 index) {
  if (data_022a6c84[index].playerCount == 0xff) {
    return data_021674b4;
  }
  return data_022a6c84[index].name;
}
