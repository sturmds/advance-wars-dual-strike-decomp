#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_022a7290;
extern u8 data_02168690[];
extern u8 data_02168658[];
extern u8 data_0216874c[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Oam_PutSprite(int x, int y, void *shape, int tileAndPalette);
void Oam_PutNumber(int x, int y, s32 value, int attr);
void func_01ff9a3c(u16 x, u16 y, void *data);
void PowerPanelRight_Draw(int x, int y, int player, int coSlot, int tile, int palette);
BOOL Player_UpdatePowerReady(int player);
BOOL CanUseSuperPower(int player, int slot);
void Sound_PlaySe(int se);

/* Draws one player's panel on the right half of the screen. During CO
 * select only the frame is drawn. Otherwise: the funds (or the hidden
 * funds mark when fog or rain hides them) and a power panel for the front
 * CO and, if any, the back CO. Then, when Player_UpdatePowerReady says so, plays
 * sound 185 if the Super CO Power can be used, else 184. */
void PowerPanelRight_DrawPlayer(int x, int y, int player, BOOL coSelect)
{
    data_022a7290 = player;
    if (coSelect) {
        Sprite_QueueAt(0, x, y, data_02168690, 0x7010, 0);
    } else {
        Oam_PutSprite((x + 516) & 0x1ff, (y + 270) & 0xff, data_02168658, 0x7024);
        if ((STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) && (STATE->players[player].visionGrids & 2) == 0) {
            func_01ff9a3c((x + 56) & 0x1ff, y + 14, data_0216874c);
        } else {
            Oam_PutNumber((x + 56) & 0x1ff, y + 14, STATE->players[player].funds, 0x7026);
        }
        PowerPanelRight_Draw(x, y, player, 0, 88, 14);
        if (STATE->players[player].back.id != 0) {
            PowerPanelRight_Draw(x, y + 20, player, 1, 96, 13);
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
