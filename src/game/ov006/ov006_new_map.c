#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/keypad.h"

void Units_ClearAll(void);
void func_020c4448(void);
void Ov006_LoadPresetMap(void);
void Ov006_GenerateRandomMap(void);
void func_ov006_0235cc6c(void);
int Ov006_FindHqs(void);
u8 Ov006_CountPropertyTiles(void);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Makes a new 30x20 map: sets the size and the row start table, then fills
 * the terrain with Ov006_GenerateRandomMap, or with Ov006_LoadPresetMap when
 * L and R are held.
 * Then it runs Ov006_RefitTileCell on every tile, rebuilds the property
 * list and records the HQs and the property count. */
void Ov006_NewMap(void) {
  int i;

  Units_ClearAll();
  data_ov006_0236d640->unk_2c[0] = 0;
  STATE->unk_1c4->mapWidth = 30;
  STATE->unk_1c4->mapHeight = 20;
  STATE->unk_1c4->scrollFlags = 0;
  for (i = 0; i < STATE->unk_1c4->mapHeight; i++) {
    STATE->mapTiles->rowStart[i] = i * STATE->unk_1c4->mapWidth;
  }
  if ((gKeypad->held & (PAD_BUTTON_L | PAD_BUTTON_R)) ==
      (PAD_BUTTON_L | PAD_BUTTON_R)) {
    Ov006_LoadPresetMap();
  } else {
    Ov006_GenerateRandomMap();
  }
  func_ov006_0235cc6c();
  func_020c4448();
  Ov006_FindHqs();
  data_ov006_0236d640->propertyCount = Ov006_CountPropertyTiles();
}
