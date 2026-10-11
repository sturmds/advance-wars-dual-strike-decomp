#include "types.h"
#include "game/script_slot.h"

void func_020c4018(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020d4124(void);

/* Script callback that ends the Start-button map overview: stops the preview,
 * then sets the fade-in counter (slot->unk_1e) that MapOverview_FadeInStep
 * counts down. */
void MapOverview_Close(ScriptSlot *slot)
{
    func_020c4018();
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    func_020d4124();
    slot->unk_1e = 6;
}
