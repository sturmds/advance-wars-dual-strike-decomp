#include "types.h"

char *func_021246d4(char *dest, const char *src);  /* strcpy */
u32 strlen(const char *str);                /* strlen */

extern const char *gLanguageSuffix;   /* "_E" in this ROM */
extern char gLocalizedPath[0x40];

/* Returns path with the language suffix appended, in a static buffer. */
char *File_LocalizePath(const char *path)
{
    func_021246d4(gLocalizedPath, path);
    func_021246d4(gLocalizedPath + strlen(gLocalizedPath), gLanguageSuffix);
    return gLocalizedPath;
}
