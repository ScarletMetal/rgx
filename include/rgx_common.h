#ifndef RGX_RGX_H
#define RGX_RGX_H
#include <stdint.h>

enum RGX_TYPE {
    RGX_EMPTY,
    RGX_BEGIN,
    RGX_END,

    RGX_LITERAL,
    RGX_SPACE,
    RGX_NON_SPACE,
    RGX_ALPHA,
    RGX_NON_ALPHA,
    RGX_DIGIT,
    RGX_NON_DIGIT,

    RGX_DOT,
    RGX_STAR,
    RGX_PLUS,
    RGX_QUESTION,
    RGX_RANGE,

    RGX_CHAR_RANGE,
    RGX_CLASS,
    RGX_GROUP,


    RGX_PATTERN,
    RGX_SEQUENCE,

    RGX_CONTAINER,

    RGX_PATTERN_END,
};

struct rgx_node {
    enum RGX_TYPE type;

    struct rgx_node *next;
};

struct rgx_literal {
    struct rgx_node base;

    char literal;
};

struct rgx_class {
    struct rgx_node base;

    struct rgx_node *items;
};

struct rgx_range {
    struct rgx_node base;

    unsigned int min;
    unsigned int max;
};

struct rgx_char_range {
    struct rgx_node base;

    char min;
    char max;
};

struct rgx_container {
    struct rgx_node base;

    struct rgx_node *child;
};

void rgx_print(struct rgx_node *n);

void rgx_print_single(struct rgx_node *n);

char *rgx_type_for(enum RGX_TYPE type);

#endif //RGX_RGX_H
