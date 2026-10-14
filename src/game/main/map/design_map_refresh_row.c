#include "types.h"
#include "game/design_map.h"

extern DesignMapRecord data_02291774;
extern DesignMap data_022a6c84[];

int func_0201fe28(u8 recordId);
int Save_LoadRecord(u8 recordId, void* buffer);
void String_Copy(char* dest, const char* src);

/* Fills row slot of data_022a6c84 from save record 7 + slot (name, player
 * count, army colours, property counts). Returns 1 on success. A slot of 4 or
 * more, or an unusable record, gets playerCount 0xff (row unused) and returns 0. */
int DesignMap_RefreshRow(u32 slot) {
  DesignMapRecord* record = &data_02291774;
  int recordId;
  u8 i;

  /* needed to match: recordId is set inside the call, after the slot test */
  if (slot >= 4 || func_0201fe28(recordId = slot + 7)) {
    data_022a6c84[slot].playerCount = 0xff;
    return 0;
  }
  Save_LoadRecord(recordId, record);
  String_Copy(data_022a6c84[slot].name, record->name);
  data_022a6c84[slot].playerCount = record->playerCount;
  data_022a6c84[slot].unk_19 = record->propertyCount;
  data_022a6c84[slot].unk_1a = record->unk_4ca;
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    data_022a6c84[slot].armyColours[i] = record->armyColours[i];
  }
  return 1;
}
