#include "types.h"
#include "game/design_map.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern DesignMapRecord data_02291774;

#define STATE (data_027c027c->unk_48[data_027c0284])

int Save_LoadRecord(u8 recordId, void* buffer);
void DesignMap_ApplyRecord(void* map, DesignMapRecord* record);
void Units_ReassignPlayerSlots(int player1, int player2, int player3,
                               int player4);
void func_020c4018(void);

/* Map editor load: reads save record 7 + slot, applies it to map and the
 * game state, then gives the four player slots the army colours the map
 * set. */
void DesignMap_LoadSlotForEdit(int slot, void* map) {
  Save_LoadRecord(slot + 7, &data_02291774);
  DesignMap_ApplyRecord(map, &data_02291774);
  Units_ReassignPlayerSlots(STATE->army[1], STATE->army[2], STATE->army[3],
                            STATE->army[4]);
  func_020c4018();
}
