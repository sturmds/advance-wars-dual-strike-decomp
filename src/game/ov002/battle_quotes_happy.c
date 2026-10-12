#include "types.h"
#include "game/battle_scene.h"
#include "game/player.h"
#include "game/co_data.h"

s32 Rng_Range(s32 min, s32 max);

/* Byte offsets of the happy lines in the battle quote text file. */
enum {
  HAPPY_QUOTE_0 = 0xe28,
  HAPPY_QUOTE_1 = 0xef4,
  HAPPY_QUOTE_2 = 0xfd8,
  HAPPY_QUOTE_3 = 0x10ac,
  HAPPY_QUOTE_4 = 0x13b8
};

/* Picks the line the CO on `side` says as its happy battle line.
 * Each CO draws at random from its own subset of the five lines.
 * Needed to match: two-line picks are switches too; if/else compiles
 * to conditional loads. */
u32 Battle_PickHappyQuote(int side) {
  switch (Ov002_gBattleScene->sides[side].coId & PLAYER_CO_ID_MASK) {
    case CO_NONE:
    case CO_NELL:
    case CO_ANDY:
    case CO_MAX:
    case CO_OLAF:
    case CO_SAMI:
    case CO_GRIT:
    case CO_KANBEI:
    case CO_SONJA:
    case CO_DRAKE:
    case CO_HACHI:
    case CO_COLIN:
    case CO_JAKE:
    case CO_RACHEL:
    case CO_JAVIER:
    case CO_GRIMM:
      switch (Rng_Range(0, 2)) {
        case 0:
          return HAPPY_QUOTE_0;
        case 1:
          return HAPPY_QUOTE_1;
        default:
          return HAPPY_QUOTE_2;
      }
    case CO_KINDLE:
    case CO_FLAK:
      switch (Rng_Range(0, 1)) {
        case 0:
          return HAPPY_QUOTE_0;
        default:
          return HAPPY_QUOTE_3;
      }
    case CO_LASH:
      switch (Rng_Range(0, 1)) {
        case 0:
          return HAPPY_QUOTE_2;
        default:
          return HAPPY_QUOTE_3;
      }
    case CO_EAGLE:
      switch (Rng_Range(0, 1)) {
        case 0:
          return HAPPY_QUOTE_0;
        default:
          return HAPPY_QUOTE_4;
      }
    case CO_VON_BOLT:
      switch (Rng_Range(0, 1)) {
        case 0:
          return HAPPY_QUOTE_3;
        default:
          return HAPPY_QUOTE_4;
      }
    case CO_JESS:
    case CO_SENSEI:
    case CO_SASHA:
      return HAPPY_QUOTE_0;
    case CO_KOAL:
    case CO_ADDER:
      return HAPPY_QUOTE_3;
    case CO_JUGGER:
    case CO_HAWKE:
      return HAPPY_QUOTE_4;
  }
  return HAPPY_QUOTE_0;
}
