#include "types.h"

char* strcpy(char* dest, const char* src);
u32 strlen(const char* str); /* strlen */

extern const char* gLanguageSuffix; /* "_E" in this ROM */
static char gLocalizedPath[0x40];

/* Returns path with the language suffix appended, in a static buffer. */
char* File_LocalizePath(const char* path) {
  strcpy(gLocalizedPath, path);
  strcpy(gLocalizedPath + strlen(gLocalizedPath), gLanguageSuffix);
  return gLocalizedPath;
}
