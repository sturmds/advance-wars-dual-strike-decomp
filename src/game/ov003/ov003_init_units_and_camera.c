#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];
extern Ov003Unit* data_ov003_02369534;

u16 Wireless_GetChildAids(void);
void Ov003Rows_ClearCells(void);
void Ov003Row_AppendCell(u8 row, int tile, int kind);
void Ov003_StartSlotPlacing(int player);
void Ov003_LoadSpawns(u32 value);
int func_ov003_0235be30(void);

#define STATE data_ov003_02364928
#define SELECTED data_ov003_02369534

/* Frees all 16 Ov003Units and clears the 80 entries of data_ov003_02369738. Sets
 * the row of the first unit to 1 (or, with data_02160ac0->unk_7c set, of unit i to
 * the row of slot i), selects the unit of the current slot, puts the scroll on the
 * tile in unk_38 and unk_39 of its row, then passes every cell of each other row's record in
 * data_02160ac0 to Ov003Row_AppendCell. Finally it calls Ov003_StartSlotPlacing for each
 * linked player and func_ov003_0235be30 up to unk_1dc times. */
void Ov003_InitUnitsAndCamera(void) {
  int j;
  Ov003Unit* unit;
  int i;
  Ov003Object* object;

  unit = data_ov003_02369538;
  object = data_ov003_02369738;
  for (i = 0; i < 16; i++, unit++) {
    unit->hp = 0;
  }
  for (i = 0; i < OV003_OBJECT_COUNT; i++, object++) {
    object->active = 0;
  }
  unit = data_ov003_02369538;
  if (data_02160ac0->unk_7c == 0) {
    unit->row = 1;
  } else {
    for (i = 0; i < 8; i++, unit++) {
      unit->row = STATE->slots[i].row;
    }
  }
  SELECTED = &data_ov003_02369538[STATE->currentSlot];
  STATE->scrollX =
      (STATE->rows[SELECTED->row - 1].unk_38 * 128 + 64 >> 3) - 128;
  STATE->scrollY = (STATE->rows[SELECTED->row - 1].unk_39 * 128 + 64 >> 3) - 96;
  Ov003Rows_ClearCells();
  for (i = 1; i < STATE->rowCount + 1; i++) {
    if (i != STATE->row) {
      for (j = 0; j < LINK_ROWS[i - 1].spawnCount; j++) {
        Ov003Row_AppendCell(i, LINK_ROWS[i - 1].cells[j].tile,
                            LINK_ROWS[i - 1].cells[j].state);
      }
    }
  }
  for (i = 0; i < 8; i++) {
    if ((Wireless_GetChildAids() | 1) & (1 << i)) {
      Ov003_StartSlotPlacing(i);
    }
  }
  Ov003_LoadSpawns(STATE->preset->unk_04);
  for (i = 0; i < STATE->unk_1dc; i++) {
    if (func_ov003_0235be30() == 0) {
      break;
    }
  }
  STATE->unk_1de = 0;
}
