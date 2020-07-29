#ifndef RGX_RGX_MATCH_H
#define RGX_RGX_MATCH_H

#include "rgx_common.h"

struct rgx_match {
    struct rgx_match *next;
    char *src;
    int size;
};

int rgx_test(struct rgx_node *pattern, char *src);

struct rgx_match *rgx_match(struct rgx_node *pattern, char *src);

#endif //RGX_RGX_MATCH_H
