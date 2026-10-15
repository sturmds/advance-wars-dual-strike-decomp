#include "types.h"
#include "sdk/msl_string.h"
#include "sdk/msl.h"

extern const char* gLanguageSuffix; /* "_E" in this ROM */
static char gLocalizedPath[0x40];

/* Returns path with the language suffix appended, in a static buffer. */
char* File_LocalizePath(const char* path) {
  strcpy(gLocalizedPath, path);
  strcpy(gLocalizedPath + strlen(gLocalizedPath), gLanguageSuffix);
  return gLocalizedPath;
}
