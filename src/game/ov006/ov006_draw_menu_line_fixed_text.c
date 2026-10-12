#include "types.h"

const void* Text_Get(u32 textId);
void Ov006_DrawMenuLine(int unused, int row, const void* text, int indent);

/* Draws menu row `row` with the text of id 0x7e000011. */
void Ov006_DrawMenuLineFixedText(int unused, int row) {
  Ov006_DrawMenuLine(unused, row, Text_Get(0x7e000011), 0);
}
