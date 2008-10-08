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
	int cursor_index;
	int length;
};

cc_arraylist *cc_arraylist_new(void);
void cc_arraylist_dispose(cc_arraylist *list);
int cc_arraylist_getLength(cc_arraylist *list);
int cc_arraylist_setCursor(cc_arraylist *list, int index);
int cc_arraylist_prevCursor(cc_arraylist *list);
int cc_arraylist_nextCursor(cc_arraylist *list);
void cc_arraylist_add(cc_arraylist *list, cc_object *addobject);
void cc_arraylist_insert(cc_arraylist *list, cc_object *insertobject);
void cc_arraylist_insertWithIndex(cc_arraylist *list, cc_object *insertobject, int index);
void cc_arraylist_remove(cc_arraylist *list);
void cc_arraylist_removeWithIndex(cc_arraylist *list, int index);
cc_object *cc_arraylist_get(cc_arraylist *list);
cc_object *cc_arraylist_getWithIndex(cc_arraylist *list, int index);
char *cc_arraylist_tocstring(cc_arraylist *list);

#endif
