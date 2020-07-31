#include <stdlib.h>
#include "rgx_common.h"
#include "rgx_compile.h"
#include "rgx_stream.h"

struct rgx_node *rgx_node_make(enum RGX_TYPE type);

struct rgx_node *rgx_literal_make(char c);

struct rgx_node *rgx_class_make();

struct rgx_node *rgx_range_make(char min, char max);

struct rgx_node *rgx_group_make();

struct rgx_node *rgx_scan_simple(struct stream *sc);

struct rgx_node *rgx_scan_class(struct stream *sc);

struct rgx_node *rgx_scan_group(struct stream *sc);

struct rgx_node *rgx_scan_one(struct stream *sc);

struct rgx_node *rgx_compile(char *src) {
    struct rgx_node *s = malloc(sizeof(struct rgx_node));
    struct rgx_node *n = s;
    s->type = RGX_PATTERN;

    struct stream st;
    stream_init(&st, src);

    while (!stream_at_end(&st)) {
        n->next = rgx_scan_one(&st);
        n = n->next;
    }
    n->next = rgx_node_make(RGX_PATTERN_END);
    return s;
}

struct rgx_node *rgx_node_make(enum RGX_TYPE type) {
    struct rgx_node *n = malloc(sizeof(struct rgx_node));
    n->type = type;
    return n;
}

struct rgx_node *rgx_literal_make(char c) {
    struct rgx_literal *l = malloc(sizeof(struct rgx_literal));
    l->base.type = RGX_LITERAL;
    l->literal = c;
    return l;
}

struct rgx_node *rgx_class_make() {
    struct rgx_class *c = malloc(sizeof(struct rgx_class));
    c->base.type = RGX_CLASS;
    c->items = rgx_node_make(RGX_SEQUENCE);
    return c;
}

struct rgx_node *rgx_range_make(char min, char max) {
    struct rgx_char_range *r = malloc(sizeof(struct rgx_char_range));
    r->base.type = RGX_CHAR_RANGE;
    r->min = min;
    r->max = max;
    return r;
}

struct rgx_node *rgx_group_make() {
    struct rgx_container *o = malloc(sizeof(struct rgx_container));
    o->base.type = RGX_GROUP;
    o->child = rgx_node_make(RGX_SEQUENCE);
    return o;
}

struct rgx_node *rgx_container_make(struct rgx_node *child) {
    struct rgx_container *c = malloc(sizeof(struct rgx_container));
    c->base.type = RGX_CONTAINER;
    c->child = child;
    return c;
}

struct rgx_node *rgx_scan_simple(struct stream *sc) {
    char c = stream_consume(sc);
    switch (c) {
        case '^': return rgx_node_make(RGX_BEGIN);
        case '$': return rgx_node_make(RGX_END);
        case '.': return rgx_node_make(RGX_DOT);
        case '+': return rgx_node_make(RGX_PLUS);
        case '*': return rgx_node_make(RGX_STAR);
        case '?': return rgx_node_make(RGX_QUESTION);
        case '\\':
            c = stream_consume(sc);
            switch (c) {
                case 's': return rgx_node_make(RGX_SPACE);
                case 'w': return rgx_node_make(RGX_ALPHA);
                case 'd': return rgx_node_make(RGX_DIGIT);
                case 'S': return rgx_node_make(RGX_NON_SPACE);
                case 'W': return rgx_node_make(RGX_NON_ALPHA);
                case 'D': return rgx_node_make(RGX_NON_DIGIT);
                default: return rgx_literal_make(c);
            }
        default:
            return rgx_literal_make(c);
    }
}

struct rgx_node *rgx_scan_class(struct stream *sc) {
    stream_consume(sc); // consume '[' at the start of the class

    struct rgx_class *class = (struct rgx_class *) rgx_class_make();
    struct rgx_node *items = class->items;
    while (stream_peek(sc) != ']') {
        char c = stream_consume(sc);
        if (stream_peek(sc) == '-') {
            char c2 = stream_skip(sc, 1);
            items->next = rgx_range_make(c, c2);
        } else {
            items->next = rgx_literal_make(c);
        }
        items = items->next;
    }
    items->next = rgx_node_make(RGX_PATTERN_END);
    stream_consume(sc); // consume ']' at the end of the class
    return class;
}

struct rgx_node *rgx_scan_group(struct stream *sc) {
    stream_consume(sc); // consume '(' at the start of the group

    struct rgx_container *group = (struct rgx_container *) rgx_group_make();
    struct rgx_node *items = group->child;

    while (stream_peek(sc) != ')') {
        struct rgx_node *p = rgx_node_make(RGX_PATTERN);
        struct rgx_node *s = p;
        while (stream_peek(sc) != ')' && stream_peek(sc) != '|') {
            p->next = rgx_scan_one(sc);
            p = p->next;
        }
        p->next = rgx_node_make(RGX_PATTERN_END);
        items->next = rgx_container_make(s);
        items = items->next;
        if (stream_peek(sc) != ')') stream_consume(sc);
    }
    items->next = rgx_node_make(RGX_PATTERN_END);
    stream_consume(sc); // consume ')' at the end of the group
    return group;
}

struct rgx_node *rgx_scan_one(struct stream *sc) {
    char c = stream_peek(sc);
    switch (c) {
        case '[':
            return rgx_scan_class(sc);
        case '(':
            return rgx_scan_group(sc);
        default:
            return rgx_scan_simple(sc);
    }
}