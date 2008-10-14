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
	cc_arraylist_node *previous;
	cc_arraylist_node *next;
};

struct cc_arraylist_properties
{
	cc_arraylist_node *front;
	cc_arraylist_node *back;
	cc_arraylist_node *cursor;
	int cursor_index;
	int length;
};

cc_arraylist *cc_arraylist_new(void);
void cc_arraylist_dispose(cc_arraylist *list);
int cc_arraylist_getLength(cc_arraylist *list);
int cc_arraylist_setCursor(cc_arraylist *list, int index);
int cc_arraylist_previousCursor(cc_arraylist *list);
int cc_arraylist_nextCursor(cc_arraylist *list);
int cc_arraylist_addAtFront(cc_arraylist *list, cc_object *addobject);
int cc_arraylist_addAtBack(cc_arraylist *list, cc_object *addobject);
int cc_arraylist_addAtCursor(cc_arraylist *list, cc_object *insertobject);
int cc_arraylist_addAt(cc_arraylist *list, cc_object *insertobject, int index);
int cc_arraylist_addDynamicAt(cc_arraylist *list, cc_object *insertobject, int index);
int cc_arraylist_removeAtCursor(cc_arraylist *list);
int cc_arraylist_removeAt(cc_arraylist *list, int index);
cc_object *cc_arraylist_getAtCursor(cc_arraylist *list);
cc_object *cc_arraylist_getAt(cc_arraylist *list, int index);
char *cc_arraylist_tocstring(cc_arraylist *list);

#endif
