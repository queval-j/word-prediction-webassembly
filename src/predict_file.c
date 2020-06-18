#include <string.h>
#include "./autocomplete.h"

static
void                    init_search_node(t_predict_search *node)
{
    node->total = 0;
    node->root = NULL;
    node->weight = 0;
}

static
int                    push_node(t_predict_search *searchNode, t_tree_node *treeNode)
{
    t_psearch_node      *newNode = NULL;

    if ((newNode = (t_psearch_node*)malloc((sizeof(t_psearch_node)))) == NULL) {
        printf("Unable to allocate memory\n");
        return -1;
    }
    searchNode->total += 1;
    newNode->c = treeNode->c;
    newNode->next = NULL;
    if (searchNode->root == NULL) {
        searchNode->root = newNode;
    } else {
        newNode->next = searchNode->root;
        searchNode->root = newNode;
    }
    if (treeNode->weight > searchNode->weight) {
        searchNode->weight = treeNode->weight;
    }
    return 0;
}

static
int                     dm_search_tree(t_predict_search *searchNode, t_tree_node *treeNode, const char *term)
{
    t_tree_node         *n = treeNode->children;
    int                 i = 0;
    while (n)
    {
        if (term[i] && n->c == term[i]) { // Found, go deeper
            if (push_node(searchNode, n) < 0) {
                return -1;
            }
            ++i;
            n = n->children;
        } else if (term[i] == '\0') { // Autopilote, go deeper
            if (push_node(searchNode, n) < 0) {
                return -1;
            }
            if (n->c == '\0') { // last node
                return 0;
            }
            n = n->children;
        } else { // nothing found
            n = n->next;
        }
    }
    return -1;
}

static
void                    clean_search(t_predict_search *searchNode)
{
    t_psearch_node      *tmp = searchNode->root;
    while (tmp)
    {
        searchNode->root = tmp->next;
        free(tmp);
        tmp = searchNode->root;
    }
}

static
t_psearch_result        *dm_build_word_from_search(t_predict_search *searchNode, int termLenght)
{
    char                *s = NULL;
    int                 len = searchNode->total;
    t_psearch_node      *tmp = searchNode->root;
    t_psearch_result    *node = NULL;
    
    if (tmp == NULL) {
        return NULL;
    }
    if ((node = malloc(sizeof(t_psearch_result))) == NULL) {
        return NULL;
    }
    if ((s = malloc(sizeof(char) * len)) == NULL) {
        free(node);
        return NULL;
    }
    node->weight = searchNode->weight;
    node->result = s;
    node->lenght = len;
    while (--len >= 0)
    {
        s[len] = tmp->c;
        tmp = tmp->next;
    }
    node->diffResult = &s[termLenght];
    return node;
}

t_psearch_result        *dm_predict(const char *str)
{
    t_dictionary        *d = get_dictionary_manager();
    t_predict_search    search;
    t_tree_node         *n = d->root;
    char                *term = strdup(str);
    char                c = 0;
    ssize_t             termLenght = strlen(term);
    term = to_lower_case(term);
    c = term[0];
    init_search_node(&search);
    while (n != NULL)
    {
        if (n->c == c) {
            if (push_node(&search, n) >= 0) {
                if (dm_search_tree(&search, n, &term[1]) >= 0) {
                    t_psearch_result *predictedWord = dm_build_word_from_search(&search, termLenght);
                    clean_search(&search);
                    return predictedWord;
                }
            }
            break;
        }
        n = n->next;
    }
    clean_search(&search);
    return NULL;
}