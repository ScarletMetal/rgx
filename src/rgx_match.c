#include <stdlib.h>
#include <stdio.h>

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

    while (!stream_at_end(&s)) {
        char *str = s.current;
        printf("current before='%s'\n", str);
        int i = rgx_test_pattern(pattern->next, &s);
        if (i == 1) {
            struct rgx_match *m = rgx_match_make(str, s.current - str + 1);
            slist_insert_end(struct rgx_match *, matches, m);
        } else {
            printf("no match\n");
            stream_consume(&s);
        }
        printf("length after=%lu\n", s.current - s.start);
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
    printf("match=");
    for (int i = 0; i < m->size; i++) {
        printf("%c", m->start[i]);
    }
    printf("\n");
}