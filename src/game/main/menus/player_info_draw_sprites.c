#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

extern MenuState gMenuState;
extern u8 data_027c0030;

void PlayerInfo_DrawCoIcons(void);
void PlayerInfo_DrawPortraitScreenSprites(void);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile, int list);
void PlayerInfo_QueuePageSprites(void);
void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);

/* Queues the sprites of the player info screen: on the screen of
 * data_027c0288.unk_00 the page sprites, plus two more on the last page
 * while its view is 1; then PlayerInfo_DrawPortraitScreenSprites on the screen of unk_04. */
void PlayerInfo_DrawSprites(void)
{
    Unk_027c0280 *saved0280;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    PlayerInfo_DrawCoIcons();
    Co_QueueSmallPictureSpriteAtTile(0x10d0, 24, 0, 0, 1);
    PlayerInfo_QueuePageSprites();
    if (gMenuState.unk_0206d67c.page == 3 && gMenuState.unk_0206d67c.unk_0a4 != 0) {
        Sprite_QueueAt(0, 64, 64, &data_027c0030, 0x4000, 0x400);
        Sprite_QueueAt(0, 128, 64, &data_027c0030, 0x4000, 0x440);
    }
    Screen_SetCurrentGfx(saved0280);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    PlayerInfo_DrawPortraitScreenSprites();
    Screen_SetCurrentGfx(saved0280);
}
