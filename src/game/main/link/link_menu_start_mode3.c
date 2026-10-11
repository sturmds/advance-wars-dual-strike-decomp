#include "types.h"
#include "game/screen_other_map.h"

void func_02102348(void);
void LinkMenu_Start(void);

/* Copies the game flags (func_02102348), then opens the link menu in link
 * mode 3 (unk_8b 3), the mode that allows 8 players. */
void LinkMenu_StartMode3(void)
{
    func_02102348();
    data_027c027c->unk_48[data_027c0284]->unk_8b = 3;
    LinkMenu_Start();
}
