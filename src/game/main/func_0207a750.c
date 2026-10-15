#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "game/Unk_0229c960.h"
#include "game/link_player_name.h"

void OwnerInfo_CopyNickname(void* buffer);

void func_0207a750(void) {
  OwnerInfo_CopyNickname(gLinkPlayerNames);
  data_0229c910.usedAids = 1;
  data_0229c910.maxChildren = data_0229c960.unk_10;
  data_0229c910.unk_17 = data_0215fe84->unk_35;
}
