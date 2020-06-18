#include <string.h>
#include "autocomplete.h"

static void             print_dictionary(void)
{
    t_dictionary    *d = get_dictionary_manager();
    printf("\nInspecting dictionary:\n");
    printf("=> Number of nodes: %d\n", d->total);
    if (d->list == NULL) {
        printf("=> list is empty\n");
    } else {
        printf("=> list has an element\n");
    }
    if (d->root == NULL) {
        printf("=> root is empty\n");
    } else {
        printf("=> root has an element\n");
    }
}

void                    my_autocomplete_debug()
{
    print_dictionary();
}

static int              _gnl_distance(const char *str)
{
    ssize_t i = -1;
    while (str[++i] && str[i] != '\n')
        ;
    
    if (str[i] == '\n' || str[i] == '\0')
        return i;
    return -1;
}

void                    my_ac_load(const char *content)
{
    ssize_t i = 0;
    ssize_t max = 0;
    if (content == NULL) {
        return;
    }
    max = strlen(content);
    while (i < max) {
        int pos = _gnl_distance(&content[i]);
        if (pos >= 0) {
            char *str = strndup(&content[i], pos);
            dm_push_text(str);
            free(str);
        } else {
            i = max;
        }
        i += pos + 1;
    }
}