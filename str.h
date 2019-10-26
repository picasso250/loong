#ifndef _STR_H_
#define _STR_H_ 1

#include "util.h"
#include "gc.h"

str *newStr();
str *newStrFromCStr(char*);
void strExtendCstr(str *st, char *s);
void strExtend(str *st, str *s);
char *cstr(str *s);

#endif