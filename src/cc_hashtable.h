#ifndef CC_HASHTABLE_H
#define CC_HASHTABLE_H

#include "cc_object.h"
#include "cc_arraylist.h"

extern int g_cc_hashtable_object_id;
typedef cc_object cc_hashtable;
typedef struct cc_hashtable_properties cc_hashtable_properties;
typedef struct cc_hashtable_node cc_hashtable_node;

struct cc_hashtable_node {
	char *key;
	cc_object *object;
};

struct cc_hashtable_properties
{
	int size;
	cc_arraylist **table;
	int count;
};

cc_hashtable *cc_hashtable_new(int size);
void cc_hashtable_dispose(cc_hashtable *table);
void cc_hashtable_add(cc_hashtable *table, char *key, cc_object *object);
void cc_hashtable_set(cc_hashtable *table, char *key, cc_object *object);
cc_object *cc_hashtable_get(cc_hashtable *table, char *key);
int cc_hashtable_exist(cc_hashtable *table, char *key);
char *cc_hashtable_tocstring(cc_hashtable *table);

#endif
