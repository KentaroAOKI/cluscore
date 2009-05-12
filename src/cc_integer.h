#ifndef CC_INTEGER_H
#define CC_INTEGER_H

#include "cc_object.h"

extern int g_cc_integer_id;
typedef cc_object cc_integer;

cc_integer *cc_integer_new(int value);
void cc_integer_release(cc_integer *object);
int cc_integer_get(cc_integer *object, int *out_value);
cc_integer *cc_integer_set(cc_integer *object, int value);
cc_integer *cc_integer_plus(cc_integer *plused_object, cc_integer *object);
cc_integer *cc_integer_minus(cc_integer *minused_object, cc_integer *object);
char *cc_integer_tocstring(cc_integer *object);
int cc_integer_compere(cc_integer *object1, cc_integer *object2);

#endif /*CC_INTEGER_H*/
