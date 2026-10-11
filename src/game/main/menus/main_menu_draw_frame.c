#include "types.h"
#include "game/main_menu.h"

void Gfx3d_InitScene(void);
void G3_RotY(int sin, int cos);
void func_0210bb9c(int sin, int cos);
void MainMenu_BuildPage0(void);
void MainMenu_BuildPage1(void);
void MainMenu_BuildPage2(void);
void MainMenu_BuildPage3(void);
void MainMenu_BuildPage4(void);
void MainMenu_BuildPage5(void);
void MainMenu_BuildPage6(void);
void MainMenu_BuildPage7(void);
void MainMenu_BuildPage8(void);
void MainMenu_BuildPage9(void);
void MainMenu_BuildPage10(void);
void MainMenu_BuildPage11(void);
void func_020308c8(void);
void MainMenu_BuildPage13(void);
void func_0202e8d0(void);
void MainMenu_DrawPage(void);
void MainMenu_AddEntryButtons(void);
void func_0202dd04(void);
void func_02028644(void);
void Gfx3d_SetOrthoCentered(void);

extern s16 data_0214d4c4[];            /* a sine and cosine pair per 16 angle units */

/* Per-frame 3D drawing of the main menu. While a page flip runs (unk_10_3
 * counts its 16 frames down), turns the page by up to a quarter turn each
 * way, with G3_RotY for slides 2 and 3 and func_0210bb9c otherwise.
 * Halfway through it builds the next page; on the last frame it puts the
 * entry buttons back. */
void MainMenu_DrawFrame(void)
{
    BOOL lastStep = FALSE;
    u8 slide;
    u16 angle;

    Gfx3d_InitScene();
    if (data_021599d0->unk_10_3 != 0) {
        data_021599d0->unk_10_3--;
        slide = data_021599d0->slide;
        if (slide == 3 || slide == 0) {
            if (data_021599d0->unk_10_3 < 8) {
                angle = data_021599d0->unk_10_3 << 11;
            } else {
                angle = (data_021599d0->unk_10_3 - 16) << 11;
            }
        } else {
            if (data_021599d0->unk_10_3 < 8) {
                angle = -data_021599d0->unk_10_3 << 11;
            } else {
                angle = -(data_021599d0->unk_10_3 - 16) << 11;
            }
        }
        if (slide == 2 || slide == 3) {
            G3_RotY(data_0214d4c4[(angle >> 4) * 2], data_0214d4c4[(angle >> 4) * 2 + 1]);
        } else {
            func_0210bb9c(data_0214d4c4[(angle >> 4) * 2], data_0214d4c4[(angle >> 4) * 2 + 1]);
        }
        if (data_021599d0->unk_10_3 == 8) {
            switch (data_021599d0->nextPage) {
            case 0:
                MainMenu_BuildPage0();
                break;
            case 6:
                MainMenu_BuildPage6();
                break;
            case 1:
                MainMenu_BuildPage1();
                break;
            case 2:
                MainMenu_BuildPage2();
                break;
            case 3:
                MainMenu_BuildPage3();
                break;
            case 4:
                MainMenu_BuildPage4();
                break;
            case 9:
                MainMenu_BuildPage9();
                break;
            case 5:
                MainMenu_BuildPage5();
                break;
            case 7:
                MainMenu_BuildPage7();
                break;
            case 8:
                MainMenu_BuildPage8();
                break;
            case 10:
                MainMenu_BuildPage10();
                break;
            case 11:
                MainMenu_BuildPage11();
                break;
            case 12:
                func_020308c8();
                break;
            case 13:
                MainMenu_BuildPage13();
                break;
            }
            func_0202e8d0();
        } else {
            MainMenu_DrawPage();
        }
        if (data_021599d0->unk_10_3 == 1) {
            lastStep = TRUE;
        }
        if (data_021599d0->unk_10_3 == 0) {
            MainMenu_AddEntryButtons();
            func_0202dd04();
        }
    } else {
        MainMenu_DrawPage();
    }
    func_02028644();
    if (lastStep == TRUE) {
        Gfx3d_SetOrthoCentered();
    }
}
