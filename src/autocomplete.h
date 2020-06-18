#include <stdio.h>
#include <stdlib.h>

#ifndef __MY_AUTOCOMPLETE_H__
#define __MY_AUTOCOMPLETE_H__

typedef struct          s_word_node t_word_node;
typedef struct          s_node_tree t_tree_node;
typedef struct          s_predict_search t_predict_search;
typedef struct          s_predict_search_node t_psearch_node;
typedef struct          s_predict_search_result t_psearch_result;

typedef struct          s_predict_search {
    t_psearch_node      *root;
    int                 weight;
    int                 total;
} t_predict_search;

typedef struct          s_predict_search_result {
    char                *result;
    char                *diffResult;
    int                 lenght;
    int                 weight;
} t_psearch_result;

typedef struct          s_predict_search_node {
    char                c;
    t_psearch_node      *next;
} t_psearch_node;

typedef struct          s_word_node {
    char                *word;
    int                 weight;
    t_word_node         *next;
}   t_word_node;

typedef struct          s_node_tree {
    char                c;
    int                 weight;
    t_tree_node         *next;
    t_tree_node         *children;
    t_tree_node         *parent;
} t_tree_node;

typedef struct          s_dictionary {
    int                 total;
    t_word_node         *list;
    t_tree_node         *root;
} t_dictionary;

/*
** Utils
*/
char                    *to_lower_case(char *str);

/*
** Predict APIs
*/
void                    my_autocomplete_debug();
void                    my_ac_load(const char *content);
t_dictionary            *get_dictionary_manager(void);
void                    dm_push_text(const char *line);
void                    dm_generate_tree();
int                     dm_search_word(const char *str);
void                    dm_print_words(void);
t_psearch_result        *dm_predict(const char *str);
void                    dm_clean_search_result(t_psearch_result *result);

/*
** WASM Interface
*/

int                     api_predict_init_dictionary(void);
int                     api_predict_has_word(const char *);
char                    *api_predict_word(const char *);
char                    *api_predict_get_words(void);

#endif