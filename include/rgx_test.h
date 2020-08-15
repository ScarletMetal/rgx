#ifndef RGX_RGX_TEST_H
#define RGX_RGX_TEST_H

#include "rgx_common.h"
#include "rgx_stream.h"

int rgx_test_str(struct rgx_node *pattern, char *src);

int rgx_test_pattern(struct rgx_node *pattern, struct stream *s);

#endif //RGX_RGX_TEST_H
