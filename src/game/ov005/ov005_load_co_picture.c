#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bbac.h"

void func_0200ac9c(void *state);
void func_0200acac(void *state);
void Co_LoadGraphics(int coId, int firstTile);
void Co_LoadPalette(int coId, int paletteSlot);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Loads the picture and palette of state->coId into the second screen
 * state (data_027c0288.unk_04): tile 0x180, palette slot 26. */
void Ov005_LoadCoPicture(Unk_ov005_0236bbac *state)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    func_0200acac(data_027c0288.unk_04);
    saved027c = data_027c027c;
    func_0200ac9c(data_027c0288.unk_04);
    Co_LoadGraphics(state->coId, 0x180);
    Co_LoadPalette(state->coId, 26);
    func_0200ac9c(saved027c);
    func_0200acac(saved0280);
}
