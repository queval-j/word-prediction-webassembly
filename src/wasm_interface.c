#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include "./autocomplete.h"

static
char *_read_file(char *path)
{
    ssize_t len = 0;
    int fd = open(path, 'r');
    char *content = strdup("");
    char buff[128];

    while ((len = read(fd, buff, 127)) > 0) {
        buff[len] = '\0';
        char *tmp = (char*)malloc(sizeof(char) * (strlen(content) + strlen(buff) + 1));
        tmp[0] = '\0';
        strcat(tmp, content);
        strcat(tmp, buff);
        free(content);
        content = tmp;
    }
    close(fd);
    return content;
}

int     api_predict_init_dictionary(void)
{
    char *content = _read_file("dictionary.mydb");

    // printf("File:\n%s\n", content);
    my_ac_load(content);
    dm_generate_tree();
    my_autocomplete_debug();
    return 0;
}

int                     api_predict_has_word(const char *word)
{
    int found = 0;

    if (word != NULL) {
        found = dm_search_word(word);
        // printf("Does \"%s\" has been found ? %s\n", word, (found == 1 ? "Yes" : "No"));
    }
    return found;
}

char                    *api_predict_word(const char *term)
{
    t_psearch_result *result = NULL;
    char *s = NULL;
    char *myTerm = NULL;
    
    if (term == NULL) {
        return NULL;
    }
    myTerm = strdup(term);
    myTerm = to_lower_case(myTerm);
    // printf("Predict word for \"%s\":\n", myTerm);
    result = dm_predict(myTerm);
    if (result != NULL) {
        // printf("=> Predict \"%s\" with a weight of %d\n", result->diffResult, result->weight);
        // printf("====> Full word: %s\n", result->result);
    } else {
        printf("=> Unable to predict\n");
        return NULL;
    }
    s = strdup(result->diffResult);
    dm_clean_search_result(result);
    free(myTerm);
    return s;
}

char                    *api_predict_get_words(void)
{
    t_dictionary *d = get_dictionary_manager();
    t_word_node *word = d->list;

    char *content = strdup("");
    char *a = NULL;
    int size = 0;
    int deep = 10;

    while (word && deep > 10)
    {
        a = content;
        size = asprintf(&content, "%s%s;%d\n", a, word->word, word->weight);
        if (size == -1) {
            printf("[ERROR] Unable to create list of words\n");
            return NULL;
        }
        if (a != content) {
            free(a);
            a = NULL;
        }
        word = word->next;
        --deep;
    }
    return content;
}
