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
 * The Red Black Tree Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_REDBLACKTREE_H
#define CC_REDBLACKTREE_H

#include "cc_object.h"

extern int g_cc_redblacktree_object_id;
typedef cc_object cc_redblacktree;

/* a result of cc_redblack */
typedef enum cc_redblacktree_status cc_redblacktree_status;

enum cc_redblacktree_status
{
	CC_REDBLACKTREE_STATUS_ERROR = -1,
	CC_REDBLACKTREE_STATUS_SUCCESS = 0,
	CC_REDBLACKTREE_STATUS_INSERTED,
	CC_REDBLACKTREE_STATUS_REPLACED,
	CC_REDBLACKTREE_STATUS_FAILURE
};

cc_redblacktree *cc_redblacktree_new();
void cc_redblacktree_release(cc_redblacktree *tree);
cc_redblacktree_status cc_redblacktree_insert(cc_redblacktree *tree,
		cc_object *key, cc_object *value);
cc_redblacktree_status cc_redblacktree_insertWithReplace(cc_redblacktree *tree,
		cc_object *key, cc_object *value);
cc_redblacktree_status cc_redblacktree_delete(cc_redblacktree *tree,
		cc_object *key);
cc_redblacktree_status cc_redblacktree_traversePreorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePreorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePreorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePreorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePreorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traverseInorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
cc_redblacktree_status cc_redblacktree_traverseInorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traverseInorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traverseInorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traverseInorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePostorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePostorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePostorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePostorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
cc_redblacktree_status cc_redblacktree_traversePostorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object));
int cc_redblacktree_contain(cc_redblacktree *tree, cc_object *key);
cc_object *cc_redblacktree_get(cc_redblacktree *tree, cc_object *key);
cc_object *cc_redblacktree_getWhenGreaterEqual(cc_redblacktree *tree,
		cc_object *key);
cc_object
		*cc_redblacktree_getWhenGreater(cc_redblacktree *tree, cc_object *key);
cc_object *cc_redblacktree_getWhenGreatest(cc_redblacktree *tree);
cc_object *cc_redblacktree_getWhenLessEqual(cc_redblacktree *tree,
		cc_object *key);
cc_object *cc_redblacktree_getWhenLess(cc_redblacktree *tree, cc_object *key);
cc_object *cc_redblacktree_getWhenLeast(cc_redblacktree *tree);
char *cc_redblacktree_tocstring(cc_redblacktree *tree);

#endif /*CC_REDBLACKTREE_H*/
