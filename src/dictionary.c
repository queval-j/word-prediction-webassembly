#include <string.h>

#include "autocomplete.h"

t_dictionary                *get_dictionary_manager(void)
{
    static t_dictionary *dictionary = NULL;
    
    if (dictionary == NULL) {
        dictionary = malloc(sizeof(t_dictionary));
        // TODO: check if malloc value
        dictionary->total = 0;
        dictionary->list = NULL;
        dictionary->root = NULL;
    }
    return dictionary;
}

static
void                        _get_word_size(int *length, int *index, const char *str)
    {
    int i = -1;
    char buff[5];

    while (str[++i] >= '0' && str[i] <= '9' && i < 4)
    {
        buff[i] = str[i];
    }
    buff[i] = '\0';
    *length = atoi(buff);
    *index = ++i;
}

static
t_word_node                 *_create_node_from_line(const char *str)
{
    int len = -1;
    int i = 0;
    int weight = 1;
    char *word = NULL;
    t_word_node *node = NULL;
    
    _get_word_size(&len, &i, str);
    word = strndup(&str[i], len);
    weight = atoi(&str[i + len + 1]);

    if (word != NULL) {
        if ((node = malloc(sizeof(t_word_node))) == NULL) {
            printf("Malloc Error _create_node\n");
            free(word);
            return NULL;
        }
        node->word = word;
        node->weight = weight;
        node->next = NULL;
        return node;
    }
    return NULL;
}

void                        dm_push_text(const char *str)
{
    t_dictionary *d = get_dictionary_manager();
    t_word_node *node = _create_node_from_line(str);

    if (node == NULL) {
        printf("[ERROR]\"%s\" has been skipped\n", str);
        return;
    }
    if (d->list == NULL) {
        d->list = node;
    } else {
        node->next = d->list;
        d->list = node;
    }
}

void                            dm_print_words(void)
{
    t_dictionary *d = get_dictionary_manager();
    t_word_node *node = d->list;

    printf("=== Words loaded ====\n");
    while (node)
    {
        printf("%s\n", node->word);
        node = node->next;
    }
}