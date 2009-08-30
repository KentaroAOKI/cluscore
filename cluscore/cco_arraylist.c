/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *
 *  http://www.cluscore.com/
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
 * The cco_arraylist Class for ClusCore.
 *
 * Author: Kentaro Aoki
 */

#include "cco_arraylist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(arraylist);

cco_arraylist *cco_arraylist_baseNew(int size)
{
	cco_arraylist *o;
	do {
		if (size < sizeof(cco_arraylist))
		{
			break;
		}
		o = (cco_arraylist *)cco_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_arraylist_baseInitialize(o);
	} while (0);
	return o;
}

void cco_arraylist_baseRelease(void *o)
{
	cco_arraylist_baseFinalize(o);
	cco_baseRelease(o);
}

void cco_arraylist_baseInitialize(cco_arraylist *o)
{
	o->baseRelease = &cco_arraylist_baseRelease;
	o->getCstring = &cco_arraylist_getCstring;

	o->arraylistFront = NULL;
	o->arraylistBack = NULL;
	o->arraylistCursor = NULL;
	o->arraylistCursorindex = 0;
	o->arraylistLength = 0;
	return;
}

void cco_arraylist_baseFinalize(cco_arraylist *o)
{
	return;
}

cco_arraylist *cco_arraylist_new()
{
	return cco_arraylist_baseNew(sizeof(cco_arraylist));
}

void cco_arraylist_release(void *o)
{
	cco_release(o);
}

char *cco_arraylist_getCstring(void *o)
{
	return strdup("cco_arraylist");
}

int cco_arraylist_length(cco_arraylist *cco_arraylist)
{
	int result = -1;

	if (cco_arraylist != NULL)
	{
		result = cco_arraylist->arraylistLength;
	}
	return result;
}

int cco_arraylist_setCursorAtFront(cco_arraylist *cco_arraylist)
{
	int result = -1;
	if (cco_arraylist != NULL)
	{
		cco_arraylist->arraylistCursor = cco_arraylist->arraylistFront;
		cco_arraylist->arraylistCursorindex = 0;
		result = cco_arraylist->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_setCursorAtBack(cco_arraylist *cco_arraylist)
{
	int result = -1;
	if (cco_arraylist != NULL)
	{
		cco_arraylist->arraylistCursor = cco_arraylist->arraylistBack;
		cco_arraylist->arraylistCursorindex = cco_arraylist->arraylistLength - 1;
		result = cco_arraylist->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_setCursorAt(cco_arraylist *cco_arraylist, int index)
{
	cco_arraylist_node *node = NULL;
	int count = -1;
	int diff_index;
	int countway;

	if (cco_arraylist != NULL)
	{
		if (cco_arraylist->arraylistFront != NULL && cco_arraylist->arraylistLength > index && index >= 0)
		{
			diff_index = index - cco_arraylist->arraylistCursorindex;
			if (cco_arraylist->arraylistCursor != NULL && index >= abs(diff_index))
			{
				countway = diff_index;
				count = cco_arraylist->arraylistCursorindex;
				node = cco_arraylist->arraylistCursor;
			}
			else
			{
				countway = 1;
				count = 0;
				node = cco_arraylist->arraylistFront;
			}
			if (countway > 0)
			{
				while (node != NULL)
				{
					if (count == index)
					{
						break;
					}
					count++;
					node = node->next;
				}
			}
			else if (countway < 0)
			{
				while (node != NULL)
				{
					count--;
					node = node->previous;
					if (count == index)
					{
						break;
					}
				}
			}
			cco_arraylist->arraylistCursorindex = count;
			cco_arraylist->arraylistCursor = node;
		}
	}
	return count;
}

int cco_arraylist_setCursorAtPrevious(cco_arraylist *cco_arraylist)
{
	int result = -1;

	if (cco_arraylist != NULL)
	{
		if (cco_arraylist->arraylistCursor != NULL)
		{
			cco_arraylist->arraylistCursor = cco_arraylist->arraylistCursor->previous;
			cco_arraylist->arraylistCursorindex --;
			result = cco_arraylist->arraylistCursorindex;
		}
	}
	return result;
}

/**
 * Moves the cursor of cco_arraylist to next.
 * If next is NULL result is -1 and cursor is NULL.
 */
int cco_arraylist_setCursorAtNext(cco_arraylist *cco_arraylist)
{
	int result = -1;

	if (cco_arraylist != NULL)
	{
		if (cco_arraylist->arraylistCursor != NULL)
		{
			cco_arraylist->arraylistCursor = cco_arraylist->arraylistCursor->next;
			cco_arraylist->arraylistCursorindex ++;
			if (cco_arraylist->arraylistCursor != NULL)
			{
				result = cco_arraylist->arraylistCursorindex;
			}
		}
	}
	return result;
}

int cco_arraylist_addAtFront(cco_arraylist *cco_arraylist, void *cco)
{
	cco_arraylist_node *node;
	int result = -1;

	if (cco_arraylist != NULL && cco != NULL)
	{
		cco_grab(cco);

		/* Sets properties in a node. */
		node = malloc(sizeof(cco_arraylist_node));
		node->object = cco;
		node->parent = cco_arraylist;
		node->previous = NULL;
		node->next = cco_arraylist->arraylistFront;

		/* Adds a node to a cco_arraylist. */
		cco_arraylist->arraylistLength ++;
		if (cco_arraylist->arraylistFront == NULL)
		{
			/* first time. */
			cco_arraylist->arraylistFront = node;
			cco_arraylist->arraylistBack = node;
			cco_arraylist->arraylistCursor = node;
		}
		else
		{
			cco_arraylist->arraylistFront->previous = node;
			cco_arraylist->arraylistFront = node;
		}
		/* update cursor */
		cco_arraylist->arraylistCursor = node;
		cco_arraylist->arraylistCursorindex = 0;
		result = cco_arraylist->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_addAtBack(cco_arraylist *cco_arraylist, void *cco)
{
	cco_arraylist_node *node;
	int result = -1;

	if (cco_arraylist != NULL && cco != NULL)
	{
		cco_grab(cco);

		/* Sets properties in node. */
		node = malloc(sizeof(cco_arraylist_node));
		node->object = cco;
		node->parent = cco_arraylist;
		node->previous = cco_arraylist->arraylistBack;
		node->next = NULL;

		/* Adds node to an arraylist. */
		cco_arraylist->arraylistLength ++;
		if (cco_arraylist->arraylistFront == NULL)
		{
			/* first time. */
			cco_arraylist->arraylistFront = node;
			cco_arraylist->arraylistBack = node;
			cco_arraylist->arraylistCursor = node;
		}
		else
		{
			cco_arraylist->arraylistBack->next = node;
			cco_arraylist->arraylistBack = node;
		}
		/* update cursor */
		cco_arraylist->arraylistCursor = node;
		cco_arraylist->arraylistCursorindex = cco_arraylist->arraylistLength - 1;
		result = cco_arraylist->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_addAtCursor(cco_arraylist *cco_arraylist, void *cco)
{
	cco_arraylist_node *node;
	cco_arraylist_node *insertnode;
	int result = -1;

	if (cco_arraylist != NULL && cco != NULL)
	{
		insertnode = cco_arraylist->arraylistCursor;
		if (insertnode != NULL)
		{
			cco_arraylist->arraylistLength ++;
			cco_grab(cco);
			node = malloc(sizeof(cco_arraylist_node));
			node->object = cco;
			node->parent = cco_arraylist;
			node->previous = insertnode->previous;
			node->next = insertnode;
			if (insertnode->previous != NULL)
			{
				insertnode->previous->next = node;
			}
			else
			{
				cco_arraylist->arraylistFront = node;
			}
			insertnode->previous = node;
			/* updates a cursor */
			cco_arraylist->arraylistCursor = node;
			result = cco_arraylist->arraylistCursorindex;
		}
		else
		{
			result = cco_arraylist_addAtBack(cco_arraylist, cco);
		}
	}
	return result;
}

int cco_arraylist_addAt(cco_arraylist *cco_arraylist, void *cco, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(cco_arraylist, index) >= 0)
	{
		result = cco_arraylist_addAtCursor(cco_arraylist, cco);
	}
	return result;
}

int cco_arraylist_addDynamicAt(cco_arraylist *cco_arraylist, void *cco, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(cco_arraylist, index) >= 0)
	{
		result = cco_arraylist_addAtCursor(cco_arraylist, cco);
	}
	else if (index >= 0)
	{
		result = cco_arraylist_addAtBack(cco_arraylist, cco);
	}
	else if (index < 0)
	{
		result = cco_arraylist_addAtFront(cco_arraylist, cco);
	}
	return result;
}

#if 0
int cco_arraylist_addWithSort(cco_arraylist *cco_arraylist, void *cco)
{
	int result;
	int index;
	result = cco_arraylist_subSort(cco_arraylist, cco, NULL, &index);
	if (result == 0)
	{
		cco_arraylist_addDynamicAt(cco_arraylist, cco, index);
	}
	return result;
}
#endif

int cco_arraylist_removeAtFront(cco_arraylist *cco_arraylist)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (cco_arraylist != NULL)
	{
		removenode = cco_arraylist->arraylistFront;
		if (removenode != NULL)
		{
			cco_arraylist->arraylistLength --;
			cco_release(removenode->object);
			cco_arraylist->arraylistFront = removenode->next;
			if (removenode->next == NULL)
			{
				cco_arraylist->arraylistBack = NULL;
			}
			else
			{
				removenode->next->previous = NULL;
			}
			/* update cursor */
			cco_arraylist->arraylistCursor = cco_arraylist->arraylistFront;
			cco_arraylist->arraylistCursorindex = 0;
			free(removenode);
			result = cco_arraylist->arraylistCursorindex;
		}
	}
	return result;
}

int cco_arraylist_removeAtBack(cco_arraylist *cco_arraylist)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (cco_arraylist != NULL)
	{
		removenode = cco_arraylist->arraylistBack;
		if (removenode != NULL)
		{
			cco_arraylist->arraylistLength --;
			cco_release(removenode->object);
			cco_arraylist->arraylistBack = removenode->previous;
			if (removenode->previous == NULL)
			{
				cco_arraylist->arraylistFront = NULL;
			}
			else
			{
				removenode->previous->next = NULL;
			}
			/* update cursor */
			cco_arraylist->arraylistCursor = cco_arraylist->arraylistBack;
			cco_arraylist->arraylistCursorindex = cco_arraylist->arraylistLength - 1;
			free(removenode);
			result = cco_arraylist->arraylistCursorindex;
		}
	}
	return result;
}

int cco_arraylist_removeAtCursor(cco_arraylist *cco_arraylist)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (cco_arraylist != NULL)
	{
		removenode = cco_arraylist->arraylistCursor;
		if (removenode != NULL)
		{
			cco_arraylist->arraylistLength --;
			cco_release(removenode->object);
			if (removenode->previous == NULL)
			{
				cco_arraylist->arraylistFront = removenode->next;
			}
			else
			{
				removenode->previous->next = removenode->next;
			}
			if (removenode->next == NULL)
			{
				cco_arraylist->arraylistBack = removenode->previous;
			}
			else
			{
				removenode->next->previous = removenode->previous;
			}
			/* updates cursor */
			cco_arraylist->arraylistCursor = removenode->next;
			free(removenode);
			result = cco_arraylist->arraylistCursorindex;
		}
	}
	return result;
}

#if 0

int cc_arraylist_removeAt(cc_arraylist *list, int index)
{
	int result = -1;

	if (cc_arraylist_setCursorAt(list, index) >= 0)
	{
		result = cc_arraylist_removeAtCursor(list);
	}
	return result;
}

int cc_arraylist_removeDynamicAt(cc_arraylist *list, int index)
{
	int result = -1;

	if (cc_arraylist_setCursorAt(list, index) >= 0)
	{
		result = cc_arraylist_removeAtCursor(list);
	}
	else if (index >= 0)
	{
		result = cc_arraylist_removeAtBack(list);
	}
	else if (index < 0)
	{
		result = cc_arraylist_removeAtFront(list);
	}
	return result;
}

cc_object *cc_arraylist_getAtFront(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL)
	{
		properties = list->properties;
		if (properties->front != NULL)
		{
			result_object = properties->front->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getAtBack(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL)
	{
		properties = list->properties;
		if (properties->back != NULL)
		{
			result_object = properties->back->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getAtCursor(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL)
	{
		properties = list->properties;
		if (properties->cursor != NULL)
		{
			result_object = properties->cursor->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getAt(cc_arraylist *list, int index)
{
	cc_arraylist_setCursorAt(list, index);
	return cc_arraylist_getAtCursor(list);
}

int cc_arraylist_findForwardFromFront(cc_arraylist *list, cc_object *object)
{
	int result = -1;
	int index;
	cc_object *cursorobject;

	index = cc_arraylist_setCursorAtFront(list);
	if (index >= 0)
	{
		do
		{
			cursorobject = cc_arraylist_getAtCursor(list);
			if (cc_object_equalsType(cursorobject, object) == 0
					&& object->compare(object, cursorobject) == 0)
			{
				result = index;
				break;
			}
			cc_object_release(cursorobject);
			cursorobject = NULL;
			index++;
		} while (cc_arraylist_setCursorAtNext(list) >= 0);
		if (cursorobject != NULL)
		{
			cc_object_release(cursorobject);
			cursorobject = NULL;
		}
	}
	return result;
}

int cc_arraylist_findBackwardFromBack(cc_arraylist *list, cc_object *object)
{
	int result = -1;
	int index;
	cc_object *cursorobject;

	index = cc_arraylist_setCursorAtBack(list);
	if (index >= 0)
	{
		do
		{
			cursorobject = cc_arraylist_getAtCursor(list);
			if (cc_object_equalsType(cursorobject, object) == 0
					&& object->compare(object, cursorobject) == 0)
			{
				result = index;
				break;
			}
			cc_object_release(cursorobject);
			cursorobject = NULL;
			index--;
		} while (cc_arraylist_setCursorAtPrevious(list) >= 0);
		if (cursorobject != NULL)
		{
			cc_object_release(cursorobject);
			cursorobject = NULL;
		}
	}
	return result;
}

int cc_arraylist_findWithSort(cc_arraylist *list, cc_object *insertobject)
{
	int result;
	int index;
	result = cc_arraylist_subSort(list, insertobject, &index, NULL);
	if (result == 0)
	{
		result = index;
	}
	return result;
}

char *cc_arraylist_tocstring(cc_arraylist *object)
{
	char *cstring = NULL;
	int cstring_length;
	int cstring_alloc_size;
	int add_alloc_size = 1024; /* It is bigger 1. */
	char *object_cstring = NULL;
	int object_cstring_length;
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	cc_arraylist_node *node_next;

	if (object != NULL && object->properties != NULL)
	{
		properties = object->properties;
		if (properties->front != NULL)
		{
			/* Initializes cstring buffer. */
			cstring = malloc(add_alloc_size);
			memset(cstring, 0, add_alloc_size);
			cstring_length = 0;
			cstring_alloc_size = add_alloc_size;
			/* Builds cstring. */
			for (node = properties->front; node != NULL; node = node_next)
			{
				node_next = node->next;
				if (node->object->tocstring == NULL)
				{
					continue;
				}
				object_cstring = node->object->tocstring(node->object);
				if (object_cstring != NULL)
				{
					object_cstring_length = strlen(object_cstring);
					if (cstring_alloc_size <= (cstring_length
							+ object_cstring_length))
					{
						cstring_alloc_size = cstring_length
								+ object_cstring_length + add_alloc_size;
						cstring = realloc(cstring, cstring_alloc_size);
					}
					memcpy(cstring + cstring_length, object_cstring,
							object_cstring_length);
					cstring_length = cstring_length + object_cstring_length;
					cstring[cstring_length] = 0;
					free(object_cstring);
				}
			}
		}
		else
		{
			cstring = strdup("");
		}
	}
	return (cstring);
}


int cc_arraylist_subSort(cc_arraylist *list, cc_object *insertobject,
		int *contain, int *insert)
{
	cc_arraylist_properties *properties;
	cc_object *currentobject;
	int compareresult;
	int result = -1;
	int scope_front;
	int scope_center;
	int scope_back;

	if (list != NULL)
	{
		if (contain != NULL)
		{
			*contain = -1;
		}
		if (insert != NULL)
		{
			*insert = -1;
		}
		properties = list->properties;
		scope_front = 0;
		scope_back = properties->length - 1;
		while (1)
		{
			scope_center = scope_front + (int) ((scope_back - scope_front) / 2);
			compareresult = 0;
			if (scope_front < scope_back)
			{
				currentobject = cc_arraylist_getAt(list, scope_center);
				compareresult = insertobject->compare(insertobject,
						currentobject);
				cc_object_release(currentobject);
				if (compareresult > 0)
				{
					scope_front = scope_center + 1;
					continue;
				}
				else if (compareresult < 0)
				{
					scope_back = scope_center;
					continue;
				}
				else
				{
					if (contain != NULL)
					{
						*contain = scope_center;
					}
					if (insert != NULL)
					{
						*insert = scope_center;
					}
					break;
				}
			}
			else if (scope_front == scope_back)
			{
				currentobject = cc_arraylist_getAt(list, scope_center);
				compareresult = insertobject->compare(insertobject,
						currentobject);
				cc_object_release(currentobject);
				if (compareresult > 0)
				{
					if (insert != NULL)
					{
						*insert = scope_center + 1;
					}
				}
				else if (compareresult < 0)
				{
					if (insert != NULL)
					{
						*insert = scope_center;
					}
				}
				else
				{
					if (contain != NULL)
					{
						*contain = scope_center;
					}
					if (insert != NULL)
					{
						*insert = scope_center;
					}
				}
				break;
			}
			else
			{
				if (insert != NULL)
				{
					*insert = 0;
				}
				break;
			}
		}
		result = 0;
	}
	return result;
}

#endif
