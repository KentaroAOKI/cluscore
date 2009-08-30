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
 * Author:
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
char *cco_arraylist_getCstring(void *o);
int cco_arraylist_func(void *cco_arraylist);

#endif /* CCO_ARRAYLIST_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_ARRAYLIST_PROPERTIES
*/
