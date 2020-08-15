#ifndef RGX_RGX_MATCH_H
#define RGX_RGX_MATCH_H

#include "rgx_common.h"

struct rgx_match {
    struct rgx_match *next;

    char *start;
    unsigned long size;
};

struct rgx_match *rgx_match_str(struct rgx_node *p, char *str);

void rgx_match_print(struct rgx_match *m);


#endif //RGX_RGX_MATCH_H
