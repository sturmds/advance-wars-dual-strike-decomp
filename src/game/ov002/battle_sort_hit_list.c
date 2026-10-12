#include "types.h"
#include "game/battle_hit_list.h"

/* Bubble sort of the list's entries by ascending key. The flags move one field at a time.
 * Needed to match: the temporaries are int, not u8. */
void Battle_SortHitList(BattleHitList* list) {
  int i;
  int pass;

  for (pass = 1; pass < list->count; pass++) {
    for (i = 0; i < list->count - pass; i++) {
      if (list->key[i] > list->key[i + 1]) {
        u16 key = list->key[i];
        int flag0;
        int flag3;
        int flag6;
        int flag7;

        list->key[i] = list->key[i + 1];
        list->key[i + 1] = key;
        flag0 = list->flags[i].cell;
        list->flags[i].cell = list->flags[i + 1].cell;
        list->flags[i + 1].cell = flag0;
        flag3 = list->flags[i].unk_3;
        list->flags[i].unk_3 = list->flags[i + 1].unk_3;
        list->flags[i + 1].unk_3 = flag3;
        flag6 = list->flags[i].unk_6;
        list->flags[i].unk_6 = list->flags[i + 1].unk_6;
        list->flags[i + 1].unk_6 = flag6;
        flag7 = list->flags[i].unk_7;
        list->flags[i].unk_7 = list->flags[i + 1].unk_7;
        list->flags[i + 1].unk_7 = flag7;
      }
    }
  }
}
