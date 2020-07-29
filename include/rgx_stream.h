#ifndef RGX_RGX_STREAM_H
#define RGX_RGX_STREAM_H

struct stream {
    char *src;
    char *current;
    char *start;
};

void stream_init(struct stream *s, char *src);

char stream_peek(struct stream *s);

char stream_peek_next(struct stream *s);

char stream_consume(struct stream *s);

char stream_skip(struct stream *s, int n);

void stream_mark(struct stream *s);

void stream_reset(struct stream *s);

int stream_match(struct stream *s, char c);

int stream_mark_distance(struct stream *s);

#endif //RGX_RGX_STREAM_H
