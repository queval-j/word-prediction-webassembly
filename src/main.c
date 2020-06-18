#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include "./autocomplete.h"

char *read_file(char *path)
{
    int     fd = open(path, 'r');
    char    *content = strdup("");
    char    buff[128];
    ssize_t len = 0;
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

static
void printPredict(const char *term)
{
    t_psearch_result *result = NULL;
    printf("Predict word for \"%s\":\n", term);
    result = dm_predict(term);
    if (result != NULL) {
        printf("=> Predict \"%s\" with a weight of %d\n", result->diffResult, result->weight);
        printf("====> Found %s\n", result->result);
    } else {
        printf("=> Unable to predict\n");
    }
    dm_clean_search_result(result);
}

int main(void)
{
    char *content = read_file("dictionary.mydb");
    printf("File:\n%s\n", content);
    printf("=> Storing words\n");
    my_ac_load(content);
    printf("=> Generating tree\n");
    dm_generate_tree();
    printf("=> Done\n");
    my_autocomplete_debug();
    dm_print_words();
    printf("==== Testing ====\n");
    int found = dm_search_word("Hello");
    printf("Does \"Hello\" has been found ? %s\n", (found == 1 ? "Yes" : "No"));
    found = dm_search_word("Hell");
    printf("Does \"Hell\" has been found ? %s\n", (found == 1 ? "Yes" : "No"));
    found = dm_search_word("Mom");
    printf("Does \"Mom\" has been found ? %s\n", (found == 1 ? "Yes" : "No"));
    // predict test
    printPredict("Hel");
    printPredict("Lok");
    printPredict("Nothing");
    printPredict("examine");
    return 0;
}