#include <ctype.h>

#include "./autocomplete.h"

char *to_lower_case(char *str)
{
    char *t = str;
    
    while (*t) {
        *t = tolower(*t);
        ++t;
    } 
    return str;
}
