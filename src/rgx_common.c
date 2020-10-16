#include <stdio.h>

#include "rgx_common.h"

char *rgx_type_strings[] = {
        "RGX_EMPTY",
        "RGX_BEGIN",
        "RGX_END",
        "RGX_LITERAL",
        "RGX_SPACE",
        "RGX_NON_SPACE",
        "RGX_ALPHA",
        "RGX_NON_ALPHA",
        "RGX_DIGIT",
        "RGX_NON_DIGIT",

        "RGX_DOT",
        "RGX_STAR",
        "RGX_PLUS",
        "RGX_QUESTION",
        "RGX_RANGE",

        "RGX_RANGE",
        "RGX_CLASS",
        "RGX_GROUP",
        "RGX_PATTERN",
        "RGX_SEQUENCE",
        "RGX_CONTAINER",
        "RGX_PATTERN_END",
};

void rgx_node_printr(struct rgx_node *n) {
    printf("%d %s ", n->type, rgx_type_strings[n->type]);

    switch (n->type) {
        case RGX_CLASS: {
            struct rgx_class *c = (struct rgx_class *) n;
            printf("\n");
            rgx_node_printr(c->items);
            break;
        }
        case RGX_GROUP: {
            struct rgx_container *or = (struct rgx_container *) n;
            printf("\n");
            rgx_node_printr(or->child);
            break;
        }

        case RGX_CONTAINER: {
            struct rgx_container *c = (struct rgx_container *) n;
            printf("\n");
            rgx_node_printr(c->child);
            break;
        }

        case RGX_RANGE: {
            struct rgx_range *r = (struct rgx_range *) n;
            printf("min=%c, max=%c\n", r->min, r->max);
            break;
        }
        case RGX_LITERAL: {
            struct rgx_literal *l = (struct rgx_literal *) n;
            printf("%c\n", l->literal);
            break;
        }
        default:
            printf("\n");
    }
    if (n->next != NULL) {
        rgx_node_printr(n->next);
    }
}

void rgx_node_print(struct rgx_node *n) {
    printf("%d %s ", n->type, rgx_type_strings[n->type]);

    switch (n->type) {
        case RGX_CLASS: {
            struct rgx_class *c = (struct rgx_class *) n;
            printf("\n");
            rgx_node_printr(c->items);
            break;
        }
        case RGX_GROUP: {
            struct rgx_container *or = (struct rgx_container *) n;
            printf("\n");
            rgx_node_printr(or->child);
            break;
        }

        case RGX_CONTAINER: {
            struct rgx_container *c = (struct rgx_container *) n;
            printf("\n");
            rgx_node_printr(c->child);
            break;
        }

        case RGX_RANGE: {
            struct rgx_range *r = (struct rgx_range *) n;
            printf("min=%c, max=%c\n", r->min, r->max);
            break;
        }
        case RGX_LITERAL: {
            struct rgx_literal *l = (struct rgx_literal *) n;
            printf("%c\n", l->literal);
            break;
        }
        default:
            printf("\n");
    }
}

char *rgx_type_str(enum RGX_TYPE type) {
    return rgx_type_strings[type];
}
