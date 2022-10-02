#include "../include/cmd_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_commands(struct CMDs cmds) {
    if (cmds.background) {
        puts("background");
    }
    for (struct ISRLinkedListNode *p = cmds.command_list->sentinal->next; p != NULL; p = p->next) {
        char **s = p->value;
        while (*s != NULL) {
            printf("%s ", *s);
            ++s;
        }
        puts("");
    };
}

int main() {
    struct CMDs cmds;
    char *input = (char *) malloc(100 * sizeof(char));
    strcpy(input, "timeX ls -la | wc -l|cut -f2 | &");
    int result = parse(input, &cmds);
    if (result < 0) {
        puts(translate_parse_error(result));
    } else {
        print_commands(cmds);
    }

    return 0;
}