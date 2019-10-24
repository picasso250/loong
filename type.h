#ifndef _TYPE_H_
#define _TYPE_H_ 1

#include "util.h"

typedef unsigned char byte; // we only use int,float,byte

#define INT 253914290
#define FLOAT 437721913
#define BYTE 687585745
#define BOOL 359348725

bool isPrimitive(char *typeStr);

#endif