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

#ifndef CCO_ARRAYLIST_H_
#define CCO_ARRAYLIST_H_

#include "cco.h"

#define CCO_ARRAYLIST_PROPERTIES \
	cco_arraylist_node *arraylistFront;\
	cco_arraylist_node *arraylistBack;\
	cco_arraylist_node *arraylistCursor;\
	int arraylistCursorindex;\
	int arraylistLength;

typedef struct cco_arraylist cco_arraylist;
typedef struct cco_arraylist_node cco_arraylist_node;

struct cco_arraylist_node
{
	cco *object;
	cco_arraylist *parent;
	cco_arraylist_node *previous;
	cco_arraylist_node *next;
};

struct cco_arraylist {
	CCO_PROPERTIES
	CCO_ARRAYLIST_PROPERTIES
};

cco_arraylist *cco_arraylist_baseNew(int size);
void cco_arraylist_baseRelease(void *cco);
void cco_arraylist_baseInitialize(cco_arraylist *cco);
void cco_arraylist_baseFinalize(cco_arraylist *cco);
cco_arraylist *cco_arraylist_new();
void cco_arraylist_release(void *cco);

int cco_arraylist_length(cco_arraylist *cco_arraylist);
int cco_arraylist_setCursorAtFront(cco_arraylist *cco_arraylist);
int cco_arraylist_setCursorAtBack(cco_arraylist *cco_arraylist);
int cco_arraylist_setCursorAt(cco_arraylist *cco_arraylist, int index);
int cco_arraylist_setCursorAtPrevious(cco_arraylist *cco_arraylist);
int cco_arraylist_setCursorAtNext(cco_arraylist *cco_arraylist);
int cco_arraylist_addAtFront(cco_arraylist *cco_arraylist, void *cco);
int cco_arraylist_addAtBack(cco_arraylist *cco_arraylist, void *cco);
int cco_arraylist_addAtCursor(cco_arraylist *cco_arraylist, void *cco);
int cco_arraylist_addAt(cco_arraylist *cco_arraylist, void *cco, int index);
int cco_arraylist_addDynamicAt(cco_arraylist *cco_arraylist, void *cco, int index);
int cco_arraylist_removeAtFront(cco_arraylist *cco_arraylist);
int cco_arraylist_removeAtBack(cco_arraylist *cco_arraylist);
int cco_arraylist_removeAtCursor(cco_arraylist *cco_arraylist);
int cco_arraylist_removeAt(cco_arraylist *cco_arraylist, int index);
int cco_arraylist_removeDynamicAt(cco_arraylist *cco_arraylist, int index);
cco *cco_arraylist_getAtFront(cco_arraylist *cco_arraylist);
cco *cco_arraylist_getAtBack(cco_arraylist *cco_arraylist);
cco *cco_arraylist_getAtCursor(cco_arraylist *cco_arraylist);
cco *cco_arraylist_getAt(cco_arraylist *cco_arraylist, int index);

#endif /* CCO_ARRAYLIST_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_ARRAYLIST_PROPERTIES
*/
