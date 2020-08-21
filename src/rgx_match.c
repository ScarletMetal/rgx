#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rgx_common.h"
#include "rgx_test.h"
#include "rgx_stream.h"
#include "rgx_match.h"
#include "slist.h"

struct rgx_match *rgx_match_make(char *start, unsigned long size);

struct rgx_match *rgx_match_str(struct rgx_node *pattern, char *str) {
    struct stream s;
    stream_init(&s, str);

    struct rgx_match *matches = NULL;

    while (!stream_at_end(&s) && (s.current - s.src) <= strlen(str)) {
        char *str = s.current;
        int i = rgx_test_stream(pattern->next, &s);
        if (i == 1) {
            struct rgx_match *m = rgx_match_make(str, s.current - str);
            slist_insert_end(struct rgx_match *, matches, m);
        } else {
            stream_consume(&s);
        }
    }
    return matches;
}

struct rgx_match *rgx_match_make(char *start, unsigned long size) {
    struct rgx_match *m = malloc(sizeof(struct rgx_match));
    m->start = start;
    m->size = size;
    m->next = NULL;
    return m;
}

void rgx_match_print(struct rgx_match *m) {
    printf("found match=");
    for (int i = 0; i < m->size; i++) {
        printf("%c", m->start[i]);
    }
    printf("\n");
}
