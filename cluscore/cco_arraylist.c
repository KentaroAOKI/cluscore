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

cco_defineClass(cco_arraylist);

cco_arraylist *cco_arraylist_baseNew(int size)
{
	cco_arraylist *o = NULL;
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

	o->arraylistFront = NULL;
	o->arraylistBack = NULL;
	o->arraylistCursor = NULL;
	o->arraylistCursorindex = 0;
	o->arraylistLength = 0;
	return;
}

void cco_arraylist_baseFinalize(cco_arraylist *o)
{
	cco_arraylist_node *node;
	cco_arraylist_node *node_next;
	if (o != NULL)
	{
		if (o->arraylistFront != NULL)
		{
			for (node = o->arraylistFront; node != NULL; node = node_next)
			{
				node_next = node->next;
				cco_release(node->object);
				free(node);
			}
		}
		o->arraylistFront = NULL;
		o->arraylistBack = NULL;
		o->arraylistCursor = NULL;
		o->arraylistCursorindex = 0;
		o->arraylistLength = 0;
	}
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

int cco_arraylist_length(cco_arraylist *o)
{
	int result = -1;

	if (o != NULL)
	{
		result = o->arraylistLength;
	}
	return result;
}

int cco_arraylist_setCursorAtFront(cco_arraylist *o)
{
	int result = -1;
	if (o != NULL)
	{
		o->arraylistCursor = o->arraylistFront;
		o->arraylistCursorindex = 0;
		result = o->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_setCursorAtBack(cco_arraylist *o)
{
	int result = -1;
	if (o != NULL)
	{
		o->arraylistCursor = o->arraylistBack;
		o->arraylistCursorindex = o->arraylistLength - 1;
		result = o->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_setCursorAt(cco_arraylist *o, int index)
{
	cco_arraylist_node *node = NULL;
	int count = -1;
	int diff_index;
	int countway;

	if (o != NULL)
	{
		if (o->arraylistFront != NULL && o->arraylistLength > index && index >= 0)
		{
			diff_index = index - o->arraylistCursorindex;
			if (o->arraylistCursor != NULL && index >= abs(diff_index))
			{
				countway = diff_index;
				count = o->arraylistCursorindex;
				node = o->arraylistCursor;
			}
			else
			{
				countway = 1;
				count = 0;
				node = o->arraylistFront;
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
			o->arraylistCursorindex = count;
			o->arraylistCursor = node;
		}
	}
	return count;
}

int cco_arraylist_setCursorAtPrevious(cco_arraylist *o)
{
	int result = -1;

	if (o != NULL)
	{
		if (o->arraylistCursor != NULL)
		{
			o->arraylistCursor = o->arraylistCursor->previous;
			o->arraylistCursorindex --;
			result = o->arraylistCursorindex;
		}
	}
	return result;
}

/**
 * Moves the cursor of cco_arraylist to next.
 * If next is NULL result is -1 and cursor is NULL.
 */
int cco_arraylist_setCursorAtNext(cco_arraylist *o)
{
	int result = -1;

	if (o != NULL)
	{
		if (o->arraylistCursor != NULL)
		{
			o->arraylistCursor = o->arraylistCursor->next;
			o->arraylistCursorindex ++;
			if (o->arraylistCursor != NULL)
			{
				result = o->arraylistCursorindex;
			}
		}
	}
	return result;
}

int cco_arraylist_addAtFront(cco_arraylist *o, void *cco)
{
	cco_arraylist_node *node;
	int result = -1;

	if (o != NULL && cco != NULL)
	{
		cco_grab(cco);

		/* Sets properties in a node. */
		node = malloc(sizeof(cco_arraylist_node));
		node->object = cco;
		node->parent = o;
		node->previous = NULL;
		node->next = o->arraylistFront;

		/* Adds a node to a cco_arraylist. */
		o->arraylistLength ++;
		if (o->arraylistFront == NULL)
		{
			/* first time. */
			o->arraylistFront = node;
			o->arraylistBack = node;
			o->arraylistCursor = node;
		}
		else
		{
			o->arraylistFront->previous = node;
			o->arraylistFront = node;
		}
		/* update cursor */
		o->arraylistCursor = node;
		o->arraylistCursorindex = 0;
		result = o->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_addAtBack(cco_arraylist *o, void *cco)
{
	cco_arraylist_node *node;
	int result = -1;

	if (o != NULL && cco != NULL)
	{
		cco_grab(cco);

		/* Sets properties in node. */
		node = malloc(sizeof(cco_arraylist_node));
		node->object = cco;
		node->parent = o;
		node->previous = o->arraylistBack;
		node->next = NULL;

		/* Adds node to an arraylist. */
		o->arraylistLength ++;
		if (o->arraylistFront == NULL)
		{
			/* first time. */
			o->arraylistFront = node;
			o->arraylistBack = node;
			o->arraylistCursor = node;
		}
		else
		{
			o->arraylistBack->next = node;
			o->arraylistBack = node;
		}
		/* update cursor */
		o->arraylistCursor = node;
		o->arraylistCursorindex = o->arraylistLength - 1;
		result = o->arraylistCursorindex;
	}
	return result;
}

int cco_arraylist_addAtCursor(cco_arraylist *o, void *cco)
{
	cco_arraylist_node *node;
	cco_arraylist_node *insertnode;
	int result = -1;

	if (o != NULL && cco != NULL)
	{
		insertnode = o->arraylistCursor;
		if (insertnode != NULL)
		{
			o->arraylistLength ++;
			cco_grab(cco);
			node = malloc(sizeof(cco_arraylist_node));
			node->object = cco;
			node->parent = o;
			node->previous = insertnode->previous;
			node->next = insertnode;
			if (insertnode->previous != NULL)
			{
				insertnode->previous->next = node;
			}
			else
			{
				o->arraylistFront = node;
			}
			insertnode->previous = node;
			/* updates a cursor */
			o->arraylistCursor = node;
			result = o->arraylistCursorindex;
		}
		else
		{
			result = cco_arraylist_addAtBack(o, cco);
		}
	}
	return result;
}

int cco_arraylist_addAt(cco_arraylist *o, void *cco, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(o, index) >= 0)
	{
		result = cco_arraylist_addAtCursor(o, cco);
	}
	return result;
}

int cco_arraylist_addDynamicAt(cco_arraylist *o, void *cco, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(o, index) >= 0)
	{
		result = cco_arraylist_addAtCursor(o, cco);
	}
	else if (index >= 0)
	{
		result = cco_arraylist_addAtBack(o, cco);
	}
	else if (index < 0)
	{
		result = cco_arraylist_addAtFront(o, cco);
	}
	return result;
}

int cco_arraylist_removeAtFront(cco_arraylist *o)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (o != NULL)
	{
		removenode = o->arraylistFront;
		if (removenode != NULL)
		{
			o->arraylistLength --;
			cco_release(removenode->object);
			o->arraylistFront = removenode->next;
			if (removenode->next == NULL)
			{
				o->arraylistBack = NULL;
			}
			else
			{
				removenode->next->previous = NULL;
			}
			/* update cursor */
			o->arraylistCursor = o->arraylistFront;
			o->arraylistCursorindex = 0;
			free(removenode);
			result = o->arraylistCursorindex;
		}
	}
	return result;
}

int cco_arraylist_removeAtBack(cco_arraylist *o)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (o != NULL)
	{
		removenode = o->arraylistBack;
		if (removenode != NULL)
		{
			o->arraylistLength --;
			cco_release(removenode->object);
			o->arraylistBack = removenode->previous;
			if (removenode->previous == NULL)
			{
				o->arraylistFront = NULL;
			}
			else
			{
				removenode->previous->next = NULL;
			}
			/* update cursor */
			o->arraylistCursor = o->arraylistBack;
			o->arraylistCursorindex = o->arraylistLength - 1;
			free(removenode);
			result = o->arraylistCursorindex;
		}
	}
	return result;
}

int cco_arraylist_removeAtCursor(cco_arraylist *o)
{
	cco_arraylist_node *removenode;
	int result = -1;

	if (o != NULL)
	{
		removenode = o->arraylistCursor;
		if (removenode != NULL)
		{
			o->arraylistLength --;
			cco_release(removenode->object);
			if (removenode->previous == NULL)
			{
				o->arraylistFront = removenode->next;
			}
			else
			{
				removenode->previous->next = removenode->next;
			}
			if (removenode->next == NULL)
			{
				o->arraylistBack = removenode->previous;
			}
			else
			{
				removenode->next->previous = removenode->previous;
			}
			/* updates cursor */
			o->arraylistCursor = removenode->next;
			free(removenode);
			result = o->arraylistCursorindex;
		}
	}
	return result;
}

int cco_arraylist_removeAt(cco_arraylist *o, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(o, index) >= 0)
	{
		result = cco_arraylist_removeAtCursor(o);
	}
	return result;
}

int cco_arraylist_removeDynamicAt(cco_arraylist *o, int index)
{
	int result = -1;

	if (cco_arraylist_setCursorAt(o, index) >= 0)
	{
		result = cco_arraylist_removeAtCursor(o);
	}
	else if (index >= 0)
	{
		result = cco_arraylist_removeAtBack(o);
	}
	else if (index < 0)
	{
		result = cco_arraylist_removeAtFront(o);
	}
	return result;
}

cco *cco_arraylist_getAtFront(cco_arraylist *o)
{
	cco *result_object = NULL;

	if (o != NULL)
	{
		if (o->arraylistFront != NULL)
		{
			result_object = o->arraylistFront->object;
			cco_grab(result_object);
		}
	}
	return result_object;
}

cco *cco_arraylist_getAtBack(cco_arraylist *o)
{
	cco *result_object = NULL;

	if (o != NULL)
	{
		if (o->arraylistBack != NULL)
		{
			result_object = o->arraylistBack->object;
			cco_grab(result_object);
		}
	}
	return result_object;
}

cco *cco_arraylist_getAtCursor(cco_arraylist *o)
{
	cco *result_object = NULL;

	if (o != NULL)
	{
		if (o->arraylistCursor != NULL)
		{
			result_object = o->arraylistCursor->object;
			cco_grab(result_object);
		}
	}
	return result_object;
}

cco *cco_arraylist_getAt(cco_arraylist *o, int index)
{
	cco_arraylist_setCursorAt(o, index);
	return cco_arraylist_getAtCursor(o);
}
