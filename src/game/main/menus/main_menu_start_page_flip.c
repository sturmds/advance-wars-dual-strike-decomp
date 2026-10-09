#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void func_0202dd28(void);
void func_02028750(void);

/* Starts a page flip: slide is 2 (left) or 3 (right), both play a sound. */
void MainMenu_StartPageFlip(int page, int slide)
{
    if ((u32)(slide - 2) <= 1) {
        Sound_PlaySe(0x4c);
    }
    data_021599d0->prevPage = data_021599d0->page;
    data_021599d0->nextPage = page;
    data_021599d0->unk_10_3 = 16;
    data_021599d0->slide = slide;
    MenuButtons_RemoveSelectable();
    func_0202dd28();
    func_02028750();
}
