/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *  Copyright (c) 2009 ClusCore
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * The Array List Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
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
cc_object *cc_arraylist_getAtFront(cc_arraylist *list);
cc_object *cc_arraylist_getAtBack(cc_arraylist *list);
cc_object *cc_arraylist_getAt(cc_arraylist *list, int index);
int cc_arraylist_findForwardFromFront(cc_arraylist *list, cc_object *object);
int cc_arraylist_findBackwardFromBack(cc_arraylist *list, cc_object *object);
int cc_arraylist_findWithSort(cc_arraylist *list, cc_object *insertobject);
char *cc_arraylist_tocstring(cc_arraylist *list);

#endif
