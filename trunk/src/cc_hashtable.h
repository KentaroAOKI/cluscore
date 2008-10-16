#ifndef CC_HASHTABLE_H
#define CC_HASHTABLE_H

#include "cc_object.h"
#include "cc_arraylist.h"

extern int g_cc_hashtable_object_id;
typedef cc_object cc_hashtable;

cc_hashtable *cc_hashtable_new(int size);
void cc_hashtable_release(cc_hashtable *table);
void cc_hashtable_add(cc_hashtable *table, char *key, cc_object *object);
void cc_hashtable_set(cc_hashtable *table, char *key, cc_object *object);
int cc_hashtable_contain(cc_hashtable *table, char *key);
cc_object *cc_hashtable_get(cc_hashtable *table, char *key);
char *cc_hashtable_tocstring(cc_hashtable *table);

#endif
