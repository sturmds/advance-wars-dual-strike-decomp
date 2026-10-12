#include "types.h"
#include "game/Unk_0201a774.h"
#include "game/rect.h"

extern Rect data_02297794;
extern Rect data_0229779c;
extern u16 gEventTextMap[][32]; /* tile map of the event text window */

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void TileMap_GetPixelPos(u16* tile, u16* tileMap, s16* pixelPos);
void ChoicePrompt_DrawCursor(Unk_0201a774* prompt);
void EventScript_GetTextWindowRect(Rect* rect);
void DialogueCursor_Animate(void);

/* First step of the two-choice prompt in the event text window: prints text
 * 0xc00002ae at the top left of the prompt's tile map, draws the cursor and
 * sets the two touch buttons (data_02297794 and data_0229779c, each 32x16)
 * beside the prompt's position, the second 32 pixels to the right of the
 * first. */
void ChoicePrompt_Init(Unk_0201a774* prompt) {
  s16 pos[2];

  Text_PrintImmediate(0, 0, (u16*)prompt->tiles, 0xc00002ae, prompt->palette,
                      0);
  ChoicePrompt_DrawCursor(prompt);
  DialogueCursor_Animate();
  EventScript_GetTextWindowRect(&data_02297794);
  EventScript_GetTextWindowRect(&data_0229779c);
  TileMap_GetPixelPos((u16*)prompt->tiles, gEventTextMap[0], pos);
  data_02297794.x += pos[0];
  data_02297794.y += pos[1];
  data_02297794.width = 32;
  data_02297794.height = 16;
  data_0229779c.width = 32;
  data_0229779c.height = 16;
  data_0229779c.x += pos[0] + 32;
  data_0229779c.y += pos[1];
}
