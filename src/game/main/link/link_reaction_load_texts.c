#include "types.h"
#include "game/reaction_text_menu_task.h"

void Mem_ZeroUnits(u32 ignored, void* dst, u32 size, u32 unitBits);
const char* Text_Get(u32 textId);
char* strcpy(char* dst, const char* src);

/* The six reaction texts, 33 bytes each. Each has a symbol of its own. */

/* Loads the six default reaction texts into data_0229da8c. */
void LinkReaction_LoadTexts(void) {
  Mem_ZeroUnits(0, &data_0229da8c, sizeof(data_0229da8c), 8);
  strcpy(data_0229da8c.texts[0], Text_Get(0x4800000a));
  strcpy(data_0229da8c.texts[1], Text_Get(0x4800000b));
  strcpy(data_0229da8c.texts[2], Text_Get(0x4800000c));
  strcpy(data_0229da8c.texts[3], Text_Get(0x4800000d));
  strcpy(data_0229da8c.texts[4], Text_Get(0x4800000e));
  strcpy(data_0229da8c.texts[5], Text_Get(0x4800000f));
}
