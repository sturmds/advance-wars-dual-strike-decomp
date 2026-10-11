#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Bg_PutTileBlock(u16 *map, int x, int y, const u8 *block, u16 tileAdd);
int Co_GetTagStars(int co, int partner);
void Co_QueueArmySmallPicture(int coId, void *dest, int paletteSlot, int army);
void Co_UnpackPicture(int coId, int firstTile);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

#define MENU (gMenuState.unk_0206d67c)

/* Needed to match: the original adds the field offset to gMenuState's
 * address at run time instead of folding it into the literal. */
#define MENU_OFFSET(field) ((u32) & ((MenuState *)0)->unk_0206d67c.field)
#define MENU_SHOWN_CO \
    (((u8 *)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[MENU_OFFSET(coShown)])
#define MENU_PLAYER_ARMY (((u8 *)&gMenuState + (MENU.player - 1))[MENU_OFFSET(playerArmy)])

/* Number of CO ids the tag partner search goes through. */
#define CO_COUNT 28

/* Player info screen: queues the shown CO's small picture to the main
 * screen's OBJ tiles. On the sub screen it unpacks the CO's picture at tile
 * 0, then the pictures of up to three tag partners (COs with tag stars) at
 * tiles 12, 24 and 36, keeping each partner's star count in unk_09d (0 for
 * unused rows). Then it puts the layer 1 block of the player's army colour
 * (one of the five rows PlayerInfo_Init copies to gMenuState, 28 bytes
 * apart) and gives the whole layer the army's palette. */
void PlayerInfo_LoadCoPictures(void)
{
    int partner;
    int found = 0;
    int i;
    const u8 *frame;
    Unk_027c0280 *saved = data_027c0280;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Co_QueueArmySmallPicture(MENU_SHOWN_CO, data_027c0280->objVram, 16, MENU.player);
    Screen_SetCurrentGfx(saved);

    saved = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Co_UnpackPicture(MENU_SHOWN_CO & 0x7f, found * 12);
    for (partner = 0; partner < CO_COUNT; partner++) {
        if (Co_GetTagStars(MENU_SHOWN_CO, partner) != 0) {
            Co_UnpackPicture(partner, (found + 1) * 12);
            MENU.unk_09d[found] = Co_GetTagStars(MENU_SHOWN_CO, partner);
            found++;
        }
    }
    for (; found < 3; found++) {
        MENU.unk_09d[found] = 0;
    }

    switch (MENU_PLAYER_ARMY) {
    case 5:
        frame = (u8 *)&gMenuState + 112;
        break;
    case 4:
        frame = (u8 *)&gMenuState + 84;
        break;
    case 3:
        frame = (u8 *)&gMenuState + 56;
        break;
    case 2:
        frame = (u8 *)&gMenuState + 28;
        break;
    default:
        frame = (u8 *)&gMenuState;
        break;
    }
    Bg_PutTileBlock(data_027c0280->layer1Map, 4, 0, frame, 0);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer1Map[i] = ((MENU_PLAYER_ARMY - 1) << 12) | (data_027c0280->layer1Map[i] & 0xfff);
    }
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrentGfx(saved);
}
