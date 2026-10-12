#include "types.h"

void Mem_ZeroUnits(u32 ignored, void* dst, u32 size, u32 unitBits);
const char* Text_Get(u32 textId);
char* strcpy(char* dst, const char* src);

/* The six reaction texts, 33 bytes each. Each has a symbol of its own. */
extern char data_0229da8c[];
extern char data_0229daad[];
extern char data_0229dace[];
extern char data_0229daef[];
extern char data_0229db10[];
extern char data_0229db31[];

/* Loads the six default reaction texts into data_0229da8c. */
void LinkReaction_LoadTexts(void) {
  Mem_ZeroUnits(0, data_0229da8c, 198, 8);
  strcpy(data_0229da8c, Text_Get(0x4800000a));
  strcpy(data_0229daad, Text_Get(0x4800000b));
  strcpy(data_0229dace, Text_Get(0x4800000c));
  strcpy(data_0229daef, Text_Get(0x4800000d));
  strcpy(data_0229db10, Text_Get(0x4800000e));
  strcpy(data_0229db31, Text_Get(0x4800000f));
}
