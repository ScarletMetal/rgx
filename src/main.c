#include <stdlib.h>
#include <stdio.h>
#include "rgx_common.h"
#include "rgx_compile.h"
#include "rgx_test.h"



int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "unable to load pattern string\n");
        exit(71);
    }
    char *pstr = argv[1];
    char *tstr = argv[2];

    struct rgx_node *p = rgx_compile(pstr);
    rgx_print(p);
    int r = rgx_test(p, tstr);
    printf("r=%d\n", r);
}