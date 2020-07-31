#include "rgx_stream.h"

void stream_init(struct stream *s, char *src) {
    s->src = src;
    s->current = src;
    s->start = src;
}

char stream_peek(struct stream *s) {
    return *s->current;
}

char stream_peek_next(struct stream *s) {
    return *(s->current+1);
}

char stream_consume(struct stream *s) {
    return *s->current++;
}

char stream_skip(struct stream *s, int n) {
    char c = *(s->current += n);
    s->current += 1;
    return c;
}

void stream_mark(struct stream *s) {
    s->start = s->current;
}

void stream_reset(struct stream *s) {
    s->current = s->start;
}

int stream_match(struct stream *s, char c) {
    if (stream_peek(s) == c) {
        stream_consume(s);
        return 1;
    }
    return 0;
}

int stream_mark_distance(struct stream *s) {
    return s->current - s->start;
}

int stream_at_start(struct stream *s) {
    return s->current == s->src;
}

int stream_at_end(struct stream *s) {
    return stream_peek(s) == '\0';
}