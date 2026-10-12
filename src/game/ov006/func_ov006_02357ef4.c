#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int DesignMap_LoadSlotForEdit(int id, u8* buf);
char* strcpy(char* dest, const char* src);
void func_020c4448(void);
void Ov006_FindHqs(void);
void Ov006_DrawNameField(void);
int Ov006_FindArmyLayout(void);
int Ov006_PackArmySlots(int restore, int hqMask);
int Ov006_CountPropertyTiles(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02357ef4(void) {
  DesignMap_LoadSlotForEdit(data_ov006_0236d640->chosenSlot,
                            &data_ov006_0236d640->unk_eb);
  Ov006_PackArmySlots(1, Ov006_FindArmyLayout());
  strcpy((char*)data_ov006_0236d640->unk_d8,
         (char*)&data_ov006_0236d640->unk_eb);
  func_020c4448();
  Ov006_FindHqs();
  data_ov006_0236d640->propertyCount = Ov006_CountPropertyTiles();
  data_ov006_0236d640->unk_00 &= ~0x1000;
  Ov006_DrawNameField();
  data_ov006_0236d640->unk_00 |= 4;
}
