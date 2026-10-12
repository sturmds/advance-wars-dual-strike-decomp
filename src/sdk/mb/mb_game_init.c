#include "types.h"
#include "sdk/mb.h"

void MIi_CpuClear16(u16 value, void *dst, u32 size);
void MIi_CpuClearFast(u32 value, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
int MbWideString_Length(const u16 *string);
BOOL MbGame_LoadIconFile(const char *path, DlPlayGameEntry *game, BOOL isChars);

/* Builds a game entry from the registry: icon (the whole icon area is cleared
 * if either file fails), names, ggid and max players. userParam may be NULL. */
void MbGame_Init(DlPlayGameEntry *game, const DlPlayGameSource *registry, const void *userParam)
{
    volatile u16 zero16 = 0;
    volatile u32 zero32;
    BOOL charsFailed;
    BOOL paletteFailed;

    MIi_CpuClear16(zero16, game, sizeof(DlPlayGameEntry));
    game->noIcon = 0;
    charsFailed = MbGame_LoadIconFile(registry->iconCharPath, game, TRUE) == 0;
    paletteFailed = MbGame_LoadIconFile(registry->iconPalettePath, game, FALSE) == 0;
    if (charsFailed | paletteFailed) {
        game->noIcon = 1;
        zero32 = 0;
        MIi_CpuClearFast(zero32, game, DLPLAY_ICON_COLOR_BYTES + DLPLAY_ICON_PIXEL_BYTES);
    }
    game->ggid = registry->ggid;
    if (userParam != NULL) {
        MIi_CpuCopy16(userParam, game->userParam, DLPLAY_GAME_EXTRA_BYTES);
    }
    game->maxPlayers = registry->maxPlayers;
    MIi_CpuCopy16(registry->gameName, game->gameName, (u16)(MbWideString_Length(registry->gameName) * 2));
    MIi_CpuCopy16(registry->gameIntro, game->gameIntro, sizeof(game->gameIntro));
    game->playerCount = 1;
    game->playerFlags = 1;
    game->sentPlayerFlags = 1;
}
