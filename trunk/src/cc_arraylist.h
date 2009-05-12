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

struct cc_arraylist_node
{
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
void cc_arraylist_release(cc_arraylist *list);
int cc_arraylist_getLength(cc_arraylist *list);
int cc_arraylist_setCursorAtFront(cc_arraylist *list);
int cc_arraylist_setCursorAtBack(cc_arraylist *list);
int cc_arraylist_setCursorAt(cc_arraylist *list, int index);
int cc_arraylist_setCursorAtPrevious(cc_arraylist *list);
int cc_arraylist_setCursorAtNext(cc_arraylist *list);
int cc_arraylist_addAtFront(cc_arraylist *list, cc_object *addobject);
int cc_arraylist_addAtBack(cc_arraylist *list, cc_object *addobject);
int cc_arraylist_addAtCursor(cc_arraylist *list, cc_object *insertobject);
int cc_arraylist_addAt(cc_arraylist *list, cc_object *insertobject, int index);
int cc_arraylist_addDynamicAt(cc_arraylist *list, cc_object *insertobject,
		int index);
int cc_arraylist_addWithSort(cc_arraylist *list, cc_object *insertobject);
int cc_arraylist_removeAtFront(cc_arraylist *list);
int cc_arraylist_removeAtBack(cc_arraylist *list);
int cc_arraylist_removeAtCursor(cc_arraylist *list);
int cc_arraylist_removeAt(cc_arraylist *list, int index);
int cc_arraylist_removeDynamicAt(cc_arraylist *list, int index);
cc_object *cc_arraylist_getAtCursor(cc_arraylist *list);
cc_object *cc_arraylist_getAt(cc_arraylist *list, int index);
int cc_arraylist_findForwardFromFront(cc_arraylist *list, cc_object *object);
int cc_arraylist_findBackwardFromBack(cc_arraylist *list, cc_object *object);
int cc_arraylist_findWithSort(cc_arraylist *list, cc_object *insertobject);
char *cc_arraylist_tocstring(cc_arraylist *list);

#endif
