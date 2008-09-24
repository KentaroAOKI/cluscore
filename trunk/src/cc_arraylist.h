/*
 * ClusCore
 * cc_arraylist
 */
#ifndef CC_ARRAYLIST_H
#define CC_ARRAYLIST_H

#include "cc_object.h"

extern int g_cc_arraylist_object_id;
typedef cc_object cc_arraylist;
typedef struct cc_arraylist_properties cc_arraylist_properties;
typedef struct cc_arraylist_node cc_arraylist_node;

struct cc_arraylist_node {
	cc_object *object;
	cc_arraylist_properties *parent;
	cc_arraylist_node *prev;
	cc_arraylist_node *next;
};

struct cc_arraylist_properties
{
	cc_arraylist_node *top;
	cc_arraylist_node *bottom;
	cc_arraylist_node *cursor;
	int length;
};

cc_arraylist *cc_arraylist_new(void);
void cc_arraylist_dispose(cc_arraylist *list);
void cc_arraylist_add(cc_arraylist *list, cc_object *object);
void cc_arraylist_set_cursor(cc_arraylist *list, int index);
cc_object *cc_arraylist_get(cc_arraylist *list);
char *cc_arraylist_tocstring(cc_arraylist *list);

#endif
