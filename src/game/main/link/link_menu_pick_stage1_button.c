#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void HelpText_Show(u32 textId, int tile);
int func_0207a260(void);
void LinkMenu_CreateMarker(u8 kind, u16 x, u16 y);
void LinkMenu_AddStage2Buttons(void);

/* A stage 1 button (0 or 1) is picked: puts marker kind 0 next to it, stores it in
 * data_0229fb30.unk_631 and builds the stage 2 buttons. The help text differs for the link
 * player whose number equals the button. */
void LinkMenu_PickStage1Button(int button)
{
    Sound_PlaySe(0x10006);
    LinkMenu_CreateMarker(0, 112, button * 16 + 56);
    data_0229fb30.unk_631 = button;
    data_0229fb30.unk_630 = 2;
    if (func_0207a260() == 0) {
        MenuButtons_RemoveSelectable();
    }
    LinkMenu_AddStage2Buttons();
    if (data_0229fb30.unk_631 == func_0207a260()) {
        HelpText_Show(0x48000005, 960);
    } else {
        HelpText_Show(0x48000006, 960);
    }
}
