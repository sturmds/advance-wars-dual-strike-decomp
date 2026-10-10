#include "types.h"
#include "game/battle_fighter_sprites.h"
#include "game/battle_fighter.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"
#include "game/BattleSpriteTask.h"

extern s8 data_ov002_02369610[];

/* One frame of the hop of an infantry or mech side: moves the fighter by the next pair of
 * offsets in data_ov002_02369610 (6 frames) and puts its place into its first sprite task. */
void BattleFighter_StepFootHop(BattleFighter *fighter)
{
    if (!fighter->active) {
        return;
    }
    switch (BATTLE_SIDE_UNIT_TYPE(fighter->side)) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
    case 27:
    case 28:
        break;
    default:
        return;
    }
    if (fighter->unk_57 == 6) {
        fighter->active = 0;
        return;
    }
    fighter->unk_38 += data_ov002_02369610[fighter->unk_57 * 2];
    fighter->unk_3a += data_ov002_02369610[fighter->unk_57 * 2 + 1];
    fighter->unk_3e += data_ov002_02369610[fighter->unk_57 * 2];
    fighter->unk_40 += data_ov002_02369610[fighter->unk_57 * 2 + 1];
    BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_3c = fighter->unk_38;
    BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_3e = fighter->unk_3a;
    BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_42 = fighter->unk_3e;
    BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_44 = fighter->unk_40;
    fighter->unk_57++;
}
