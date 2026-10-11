#include "types.h"
#include "game/keypad.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void *File_Load(const char *path);
void ScriptSlot_ClearCallback(int slot);
void func_0201e6e8(int x, int y, int width, int height);
void Font_SetDigitEndTile(int tile);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Ov006_QueueMenuSprites(void);
void Ov006_AddScreen0Button(void);
void Ov006_LoadSlotButtonGraphics(void);
void Ov006_RemoveScreen0Button241(void);
void func_ov006_02355514(void);
void func_ov006_02355444(void);
int func_ov006_02359030(void);
void Ov006_DrawTextPage2(void);
void Ov006_DrawTextPage1(void);
void Ov006_QueuePage2Icons(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern const char data_ov006_0236c0c8[];
extern const char data_ov006_0236c0d4[];
extern const char data_ov006_0236c0d8[];

/* Screen of request 6: two pages of text (states 0-1 and 10-11). On the
 * first frame it adds the screen 0 buttons and loads /mapedit/00b into
 * sprite VRAM and /mapedit/00c (drawn on the second page) on screen 0.
 * A turns the page (func_ov006_02359030), B leaves (state 30). */
void Ov006_UpdateTextPages(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (data_ov006_0236d640->unk_00 & 0x8000) {
        data_ov006_0236d640->unk_00 &= ~0x8000;
        data_ov006_0236d640->screenState = 0;
        func_0201e6e8(0, 0, 32, 24);
        Font_SetDigitEndTile(69);
        func_ov006_02355514();
        Ov006_AddScreen0Button();
        Ov006_LoadSlotButtonGraphics();
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        FS_ChangeDir(data_ov006_0236c0c8);
        File_UnpackTo(data_027c0280->objVram + 0x4940, data_ov006_0236c0d4);
        data_ov006_0236d640->unk_74 = File_Load(data_ov006_0236c0d8);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    switch (data_ov006_0236d640->screenState) {
    case 0:
        Ov006_DrawTextPage1();
        data_ov006_0236d640->screenState = 1;
    case 1:
        Ov006_QueueMenuSprites();
        if (gKeypad->pressed & PAD_BUTTON_B) {
            data_ov006_0236d640->screenState = 30;
        }
        if (gKeypad->pressed & PAD_BUTTON_A) {
            func_ov006_02359030();
        }
        break;
    case 10:
        Ov006_DrawTextPage2();
        data_ov006_0236d640->screenState = 11;
    case 11:
        Ov006_QueuePage2Icons();
        if (gKeypad->pressed & PAD_BUTTON_B) {
            data_ov006_0236d640->screenState = 30;
        }
        if (gKeypad->pressed & PAD_BUTTON_A) {
            func_ov006_02359030();
        }
        break;
    case 30:
        if (!(gKeypad->pressed & PAD_BUTTON_B)) {
            *data_027c027c->unk_40 = 1;
        }
        func_ov006_02355444();
        Ov006_RemoveScreen0Button241();
        Gfx_ClearLayerMap(0);
        Gfx_ClearLayerMap(1);
        Gfx_RequestLayerCopy(0);
        Gfx_RequestLayerCopy(1);
        func_020c4018();
        ScriptSlot_ClearCallback(*data_027c027c->scriptSlotCurrent & 0xff);
        break;
    }
}
