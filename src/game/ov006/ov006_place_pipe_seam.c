#include "types.h"
#include "game/terrain.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_GetTilePropertyKind(int x, int y); /* 0: no property */
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */
void Ov006_AddProperty(int x, int y, int cell);
void func_020c4448(void);
int Ov006_GetPipeSeamCell(int x, int y);

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define PROPERTY_LIMIT 60

/* Puts a pipe seam on tile x, y with the cell Ov006_GetPipeSeamCell picks.
 * With 60 property tiles on the map already, it only goes ahead when the
 * tile is itself a property. */
void Ov006_PlacePipeSeam(int x, int y) {
  int cell;

  if (data_ov006_0236d640->propertyCount >= PROPERTY_LIMIT &&
      Ov006_GetTilePropertyKind(x, y) == 0) {
    return;
  }
  Ov006_SetTerrain(x, y, TERRAIN_PIPE_SEAM);
  cell = Ov006_GetPipeSeamCell(x, y);
  Ov006_StoreMapCell(x, y, cell);
  Ov006_AddProperty(x, y, cell);
  func_020c4448();
}
