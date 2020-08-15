#ifndef RGX_SLIST_H
#define RGX_SLIST_H

#define slist_insert_end(t, n, v)                \
    do {                                         \
        if (n != NULL) {t p = n;                 \
            while (p->next != NULL) p = p->next; \
            printf("setting\n");                 \
            p->next = v;                         \
        } else {                                 \
            n = v;                               \
        }                                        \
    } while (0)
#endif //RGX_SLIST_H
