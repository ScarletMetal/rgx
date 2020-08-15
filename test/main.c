#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rgx_compile.h"
#include "rgx_test.h"
#include "rgx_match.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: mode [m|t] pattern_string target_string\n");
        exit(71);
    }

    char mode = argv[1][0];
    char *pstr = argv[2];
    char *tstr = argv[3];

    struct rgx_node *pattern = rgx_compile(pstr);
    if (mode == 'm') {
        struct rgx_match *matches = rgx_match_str(pattern, tstr);
        struct rgx_match *b = matches;
        while (b != NULL) {
           rgx_match_print(b);
           b = b->next;
        }
    } else if (mode == 't') {
        int i = rgx_test_str(pattern, tstr);
        if (i) {
            printf("found pattern %s in string %s\n", pstr, tstr);
        } else {
            printf("pattern %s not found in string %s\n", pstr, tstr);
        }
    } else {
        fprintf(stderr, "Unknown Mode %c\n", mode);
        exit(71);
    }
}