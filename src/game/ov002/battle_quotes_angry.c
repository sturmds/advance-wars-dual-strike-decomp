#include "types.h"
#include "game/battle_scene.h"
#include "game/player.h"
#include "game/co_data.h"

s32 Rng_Range(s32 min, s32 max);

/* Byte offsets of the angry lines in the battle quote text file. */
enum {
  ANGRY_QUOTE_0 = 0x1168,
  ANGRY_QUOTE_1 = 0x1228,
  ANGRY_QUOTE_2 = 0x12ec,
  ANGRY_QUOTE_3 = 0x13b8
};

/* Picks the line the CO on `side` says as its angry battle line.
 * Each CO draws at random from its own subset of the four lines;
 * Jake has no case and always gets the last one.
 * Needed to match: two-line picks are switches too; if/else compiles
 * to conditional loads. */
u32 Battle_PickAngryQuote(int side) {
  switch (Ov002_gBattleScene->sides[side].coId & PLAYER_CO_ID_MASK) {
    case CO_NONE:
    case CO_OLAF:
    case CO_SAMI:
    case CO_KANBEI:
    case CO_SONJA:
    case CO_DRAKE:
    case CO_HACHI:
    case CO_COLIN:
    case CO_RACHEL:
    case CO_JAVIER:
    case CO_GRIMM:
      switch (Rng_Range(0, 2)) {
        case 0:
          return ANGRY_QUOTE_0;
        case 1:
          return ANGRY_QUOTE_1;
        default:
          return ANGRY_QUOTE_2;
      }
    case CO_SENSEI:
      switch (Rng_Range(0, 2)) {
        case 0:
          return ANGRY_QUOTE_1;
        case 1:
          return ANGRY_QUOTE_2;
        default:
          return ANGRY_QUOTE_3;
      }
    case CO_ANDY:
    case CO_MAX:
    case CO_LASH:
    case CO_FLAK:
      switch (Rng_Range(0, 1)) {
        case 0:
          return ANGRY_QUOTE_0;
        default:
          return ANGRY_QUOTE_2;
      }
    case CO_EAGLE:
    case CO_JESS:
    case CO_SASHA:
      switch (Rng_Range(0, 1)) {
        case 0:
          return ANGRY_QUOTE_0;
        default:
          return ANGRY_QUOTE_3;
      }
    case CO_NELL:
    case CO_KINDLE:
      switch (Rng_Range(0, 1)) {
        case 0:
          return ANGRY_QUOTE_1;
        default:
          return ANGRY_QUOTE_2;
      }
    case CO_ADDER:
      switch (Rng_Range(0, 1)) {
        case 0:
          return ANGRY_QUOTE_1;
        default:
          return ANGRY_QUOTE_3;
      }
    case CO_GRIT:
      switch (Rng_Range(0, 1)) {
        case 0:
          return ANGRY_QUOTE_2;
        default:
          return ANGRY_QUOTE_3;
      }
    case CO_VON_BOLT:
    case CO_JUGGER:
    case CO_KOAL:
    case CO_HAWKE:
      return ANGRY_QUOTE_3;
  }
  return ANGRY_QUOTE_3;
}
