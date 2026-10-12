#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/co_data.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Rank of a CO for the AI's tag choice: AiCo_FrontCloserToSuper compares the
 * front and back CO's ranks when their meters do not decide. Most COs rank 2. Kanbei
 * ranks 0, and Hachi 6 and Colin 7, only while gAiTurnState->factoryDomainsOwn
 * is nonzero; Sonja ranks 3 in Fog of War or rain. */
int AiCo_TagRank(int coId)
{
    /* needed to match: one return per case, in this order */
    switch (coId) {
    case CO_NELL:
        return 2;
    case CO_ANDY:
        return 2;
    case CO_MAX:
        return 1;
    case CO_SAMI:
        return 4;
    case CO_HACHI:
        if (gAiTurnState->factoryDomainsOwn != 0) {
            return 6;
        }
        return 2;
    case CO_JAKE:
        return 2;
    case CO_RACHEL:
        return 2;
    case CO_OLAF:
        return 2;
    case CO_GRIT:
        return 5;
    case CO_COLIN:
        if (gAiTurnState->factoryDomainsOwn != 0) {
            return 7;
        }
        return 2;
    case CO_SASHA:
        return 2;
    case CO_KANBEI:
        if (gAiTurnState->factoryDomainsOwn != 0) {
            return 0;
        }
        return 2;
    case CO_SONJA:
        if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
            return 3;
        }
        return 2;
    case CO_SENSEI:
        return 2;
    case CO_GRIMM:
        return 2;
    case CO_EAGLE:
        return 2;
    case CO_DRAKE:
        return 2;
    case CO_JESS:
        return 2;
    case CO_JAVIER:
        return 2;
    case CO_LASH:
        return 2;
    case CO_HAWKE:
        return 2;
    case CO_JUGGER:
        return 2;
    case CO_KOAL:
        return 2;
    case CO_KINDLE:
        return 2;
    case CO_VON_BOLT:
        return 2;
    case CO_NONE:
        return 2;
    default:
        return 2;
    }
}
