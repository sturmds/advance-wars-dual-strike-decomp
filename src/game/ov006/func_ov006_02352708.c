#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadCoIcons(int arg);
void Ov006_LoadTerrainTiles(int arg);

extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02352708(int arg) {
  if (arg == 1) {
    Ov006_LoadCoIcons(data_ov006_0236d640->unitPage);
  } else if (arg == 0) {
    Ov006_LoadTerrainTiles(data_ov006_0236d640->terrainPage);
  }
}
