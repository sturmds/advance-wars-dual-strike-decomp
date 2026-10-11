#include "types.h"

void Mem_ZeroUnits(u32 ignored, void *dst, u32 size, u32 unitBits);
const char *Text_Get(u32 textId);
char *func_021246d4(char *dst, const char *src);

/* The six reaction texts, 33 bytes each. Each has a symbol of its own. */
extern char data_0229da8c[];
extern char data_0229daad[];
extern char data_0229dace[];
extern char data_0229daef[];
extern char data_0229db10[];
extern char data_0229db31[];

/* Loads the six default reaction texts into data_0229da8c. */
void LinkReaction_LoadTexts(void)
{
    Mem_ZeroUnits(0, data_0229da8c, 198, 8);
    func_021246d4(data_0229da8c, Text_Get(0x4800000a));
    func_021246d4(data_0229daad, Text_Get(0x4800000b));
    func_021246d4(data_0229dace, Text_Get(0x4800000c));
    func_021246d4(data_0229daef, Text_Get(0x4800000d));
    func_021246d4(data_0229db10, Text_Get(0x4800000e));
    func_021246d4(data_0229db31, Text_Get(0x4800000f));
}
