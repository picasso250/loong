#include <stdbool.h>

#include "type.h"

bool isPrimitive(char *typeStr)
{
    return (strcmp(typeStr, "int") == 0) ||
           (strcmp(typeStr, "unsigned") == 0) ||
           (strcmp(typeStr, "short") == 0) ||
           (strcmp(typeStr, "long") == 0) ||
           (strcmp(typeStr, "float") == 0) ||
           (strcmp(typeStr, "double") == 0) ||
           (strcmp(typeStr, "char") == 0) ||
           (strcmp(typeStr, "byte") == 0) ||
           (strcmp(typeStr, "bool") == 0);
}