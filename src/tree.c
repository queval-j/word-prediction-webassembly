#include <string.h>

#include "autocomplete.h"

static
t_tree_node             *create_node(int weight, char c, t_tree_node *parent)
{
    t_tree_node         *tNode = (t_tree_node*)malloc(sizeof(t_tree_node));
    tNode->c = c;
    tNode->weight = weight;
    tNode->next = NULL;
    tNode->children = NULL;
    tNode->parent = parent;
    return tNode;
}

void                    dm_clean_search_result(t_psearch_result *result)
{
    if (result != NULL) {
        free(result->result);
        free(result);
    }
}

static
void                    node_sort_children_by_weight(t_tree_node *parent)
{
    t_tree_node         *a = NULL;
    t_tree_node         *b = NULL;
    t_tree_node         *c = NULL;
    
    a = parent->children;
    while (a != NULL)
    {
        b = a->next;
        if (b != NULL && b->weight > a->weight)
        {
            if (b->next != NULL) {
                c = b->next;
            }
            b->next = a;
            a->next = c;
            if (parent->children == a) {
                parent->children = b;
            }
            a = b;
            b = NULL;
            c = NULL;
        } else {
            a = b;
        }
    }
}

static
void                    push_node(t_dictionary *d, t_word_node *wordNode, t_tree_node *parent, const char *str)
{
    t_tree_node *tmp = parent->children;
    t_tree_node *node = NULL;
    char        c = str[0];
    d->total += 1;
    if (tmp == NULL) {
        node = create_node(wordNode->weight, c, parent);
        parent->children = node;
        if (c != '\0') {
            push_node(d, wordNode, node, &str[1]);
            return;
        }
        return;
    }
    while (tmp)
    {
        if (tmp->c == c) {
            if (c != '\0') {
                push_node(d, wordNode, tmp, &str[1]);
                node_sort_children_by_weight(tmp);
            }
            return;
        }
        else if (tmp->next == NULL) {
            node = create_node(wordNode->weight, c, parent);
            tmp->next = node;
            // Continue to add
            if (c != '\0') {
                push_node(d, wordNode, node, &str[1]);
            }
            return;
        }
        tmp = tmp->next;
    }
}

static
void                    push_word(t_dictionary *d, t_word_node *wordNode)
{
    t_tree_node         *tmp = d->root;
    char                c = wordNode->word[0];
    t_tree_node         *node = NULL;
    if (tmp == NULL) {
        node = create_node(wordNode->weight, c, NULL);
        d->root = node;
        d->total += 1;
        push_node(d, wordNode, node, &wordNode->word[1]);
    } else {
        while (tmp)
        {
            if (tmp->c == c) {
                d->total += 1;
                if (tmp->weight < wordNode->weight) {
                    tmp->weight = wordNode->weight;
                }
                push_node(d, wordNode, tmp, &wordNode->word[1]);
                return;
            }
            else if (tmp->next == NULL) {
                node = create_node(wordNode->weight, c, NULL);
                tmp->next = node;
                d->total += 1;
                push_node(d, wordNode, node, &wordNode->word[1]);
                return;
            }
            tmp = tmp->next;
        }
    }
}

void                    dm_generate_tree(void)
{
    t_dictionary *d = get_dictionary_manager();
    t_word_node *node = d->list;
    while (node != NULL)
    {
        push_word(d, node);
        node = node->next;
    }
}

int                     dm_search_word(const char *term)
{
    t_dictionary    *d = get_dictionary_manager();
    int             index = 0;
    char            *str = strdup(term);
    char            c = 0;
    t_tree_node     *node = d->root;

    str = to_lower_case(str);
    c = str[index];
    while (node)
    {
        if (node->c == c) {
            c = str[++index];
            if (node->children == NULL) {
                return 1;
            }
            node = node->children;
            continue;
        }
        node = node->next;
    }
    return 0;
}