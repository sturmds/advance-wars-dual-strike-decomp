#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 gPowerPanelPlayer;
extern u8 data_021686b0[];
extern u8 data_02168658[];
extern u8 data_0216874c[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Oam_PutSprite(int x, int y, void *shape, int tileAndPalette);
void Oam_PutNumber(int x, int y, s32 value, int attr);
void Oam_PutTextString(u16 x, u16 y, void *data);
void PowerPanelLeft_Draw(int x, int y, int player, int coSlot, int tile, int palette);
BOOL Player_UpdatePowerReady(int player);
BOOL CanUseSuperPower(int player, int slot);
void Sound_PlaySe(int se);

/* Left-half twin of PowerPanelRight_DrawPlayer: the frame only during CO
 * select, otherwise funds (or the hidden-funds mark under fog or rain) and
 * the power panels of the front and back CO. Then plays sound 185 if the
 * Super CO Power can be used, else 184, when Player_UpdatePowerReady says so. */
void PowerPanelLeft_DrawPlayer(int x, int y, int player, BOOL coSelect)
{
    gPowerPanelPlayer = player;
    if (coSelect) {
        Sprite_QueueAt(0, x, y, data_021686b0, 0x7010, 0);
    } else {
        Oam_PutSprite((x + 512) & 0x1ff, (y + 270) & 0xff, data_02168658, 0x7024);
        if ((STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) && (STATE->players[player].visionGrids & 2) == 0) {
            Oam_PutTextString((x + 52) & 0x1ff, y + 14, data_0216874c);
        } else {
            Oam_PutNumber((x + 52) & 0x1ff, y + 14, STATE->players[player].funds, 0x7026);
        }
        PowerPanelLeft_Draw(x, y, player, 0, 88, 14);
        if (STATE->players[player].back.id != 0) {
            PowerPanelLeft_Draw(x, y + 20, player, 1, 96, 13);
        }
    }
    if (!Player_UpdatePowerReady(player)) {
        return;
    }
    if (CanUseSuperPower(player, 0)) {
        Sound_PlaySe(185);
    } else {
        Sound_PlaySe(184);
    }
}
