/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *  Copyright (c) 2009 ClusCore
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
 * The cco_redblacktree Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_REDBLACKTREE_H_
#define CCO_REDBLACKTREE_H_

#include "cco.h"
#include "cco_v.h"

#define CCO_REDBLACKTREE_PROPERTIES \
	cco_redblacktree_node *redblacktreeRoot;\
	cco_redblacktree_node *redblacktreeCursor;

typedef struct cco_redblacktree cco_redblacktree;
typedef struct cco_redblacktree_node cco_redblacktree_node;
typedef enum cco_redblacktree_color cco_redblacktree_color;
typedef enum cco_redblacktree_status cco_redblacktree_status;

enum cco_redblacktree_color
{
	CCO_REDBLACKTREE_COLOR_RED, CCO_REDBLACKTREE_COLOR_BLACK
};

enum cco_redblacktree_status
{
	CCO_REDBLACKTREE_STATUS_ERROR = -1,
	CCO_REDBLACKTREE_STATUS_SUCCESS = 0,
	CCO_REDBLACKTREE_STATUS_INSERTED,
	CCO_REDBLACKTREE_STATUS_REPLACED,
	CCO_REDBLACKTREE_STATUS_FAILURE
};

struct cco_redblacktree_node
{
	cco_v *key;
	cco_redblacktree_node *left_node;
	cco_redblacktree_node *right_node;
	cco_redblacktree_node *parent_node;
	cco_redblacktree_color color;
	cco *object;
};

struct cco_redblacktree {
	CCO_PROPERTIES
	CCO_REDBLACKTREE_PROPERTIES
};

cco_redblacktree *cco_redblacktree_baseNew(int size);
void cco_redblacktree_baseRelease(void *cco);
void cco_redblacktree_baseInitialize(cco_redblacktree *cco);
void cco_redblacktree_baseFinalize(cco_redblacktree *cco);
cco_redblacktree *cco_redblacktree_new();
void cco_redblacktree_release(void *cco);

#endif /* CCO_REDBLACKTREE_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_REDBLACKTREE_PROPERTIES
*/
