#include <stdio.h>
#include <ctype.h>
#include "rgx_compile.h"
#include "rgx_stream.h"

#define rgx_pattern_at_end(p, s) ((stream_peek(s) == 0) || p->type == RGX_PATTERN_END)

#define in_range(i, min, max) (i <= max && i >= min)

int rgx_test_simple(struct rgx_node *item, struct stream *s);

int rgx_test_class(struct rgx_node *item, struct stream *s);

int rgx_test_group(struct rgx_node *item, struct stream *s);

int rgx_is_literal(struct rgx_node *item, char c);

int rgx_in_range(struct rgx_node *item, char c);

int rgx_test_one(struct rgx_node *item, struct stream *s);

int rgx_test_range(struct rgx_node *item, struct stream *s, int min, int max);

int rgx_test_pattern(struct rgx_node *item, struct stream *s);

int rgx_test(struct rgx_node *pattern, char *src) {
    struct stream s;
    stream_init(&s, src);
    return rgx_test_pattern(pattern->next, &s);
}

int rgx_test_one(struct rgx_node *item, struct stream *s) {
    stream_mark(s);
    int i;
    switch (item->type) {
        case RGX_GROUP:
            i = rgx_test_group(item, s);
            break;
        case RGX_CLASS:
            i = rgx_test_class(item, s);
            break;
        default:
            i = rgx_test_simple(item, s);
            break;
    }

    if (i == 0) stream_reset(s);
    return i;
}

int rgx_test_pattern(struct rgx_node *item, struct stream *s) {
    while (!rgx_pattern_at_end(item, s)) {
        struct rgx_node *next = item->next;
        switch (next->type) {
            case RGX_QUESTION: {
                int i = rgx_test_range(item, s, 0, 1);
                if (i == 0) return 0;
                item->next = next->next;
                break;
            }
            case RGX_STAR: {
                int i = rgx_test_range(item, s, 0, 0);
                if (i == 0) return 0;
                item->next = next->next;
                break;
            }
            case RGX_PLUS: {
                int i = rgx_test_range(item, s, 1, 0);
                if (i == 0) return 0;
                item->next = next->next;
                break;
            }
            default: {
                int i = rgx_test_one(item, s);
                if (i == 0) return 0;
            }
        }
        item = item->next;
    }
    if (stream_peek(s) == 0 && item->type != RGX_PATTERN_END) return 0;
    return 1;
}

int rgx_test_simple(struct rgx_node *item, struct stream *s) {
    char c = stream_consume(s);
    switch (item->type) {
        case RGX_DOT:
            return 1;
        case RGX_SPACE:
            return isspace(c);
        case RGX_ALPHA:
            return isalpha(c);
        case RGX_DIGIT:
            return isdigit(c);
        case RGX_NON_SPACE:
            return !isspace(c);
        case RGX_NON_ALPHA:
            return !isalpha(c);
        case RGX_NON_DIGIT:
            return !isdigit(c);
        case RGX_LITERAL:
            return rgx_is_literal(item, c);
        default:
            return 0;
    }
}

int rgx_test_class(struct rgx_node *item, struct stream *s) {
    struct rgx_class *class = (struct rgx_class *) item;
    struct rgx_node *seq = class->items->next;
    char c = stream_consume(s);
    while (seq != NULL) {
        switch (seq->type) {
            case RGX_LITERAL:
                if (rgx_is_literal(seq, c))
                    return 1;
                break;
            case RGX_CHAR_RANGE:
                if (rgx_in_range(seq, c))
                    return 1;
                break;
            default:
                return 0;
        }
        seq = seq->next;
    }
    return 0;
}


int rgx_test_range(struct rgx_node *item, struct stream *s, int min, int max) {
    int i = 0;
    int r = 1;
    while (!rgx_pattern_at_end(item, s) && r == 1) {
        r = rgx_test_one(item, s);
        if (r == 1) i++;
    }
    if (max <= min) return i >= min;
    return in_range(i, min, max);
}

int rgx_test_group(struct rgx_node *item, struct stream *s) {
    struct rgx_container *group = (struct rgx_container *) item;
    struct rgx_container *child = (struct rgx_container *) group->child->next;
    struct rgx_node *b = (struct rgx_node *) child;

    while (!rgx_pattern_at_end(b, s)) {
        if (rgx_test_pattern(child->child->next, s)) return 1;
        b = b->next;
        child = (struct rgx_container *) b;
    }
    return 0;
}

int rgx_is_literal(struct rgx_node *item, char c) {
    struct rgx_literal *l = (struct rgx_literal *) item;
    return l->literal == c;
}

int rgx_in_range(struct rgx_node *item, char c) {
    struct rgx_char_range *r = (struct rgx_char_range *) item;
    return in_range(c, r->min, r->max);
}