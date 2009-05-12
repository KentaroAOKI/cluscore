/*
 * cc_binarytree.h
 *
 *  Created on: Nov 21, 2008
 *      Author: kentaro
 */

#ifndef CC_BINARYTREE_H
#define CC_BINARYTREE_H

#include "cc_object.h"

extern int g_cc_binarytree_object_id;
typedef cc_object cc_binarytree;

cc_binarytree *cc_binarytree_new();
void cc_binarytree_release(cc_binarytree *tree);
void cc_binarytree_insert(cc_binarytree *tree, cc_object *key,
		cc_object *object);
void cc_binarytree_delete(cc_binarytree *tree, cc_object *key);
void cc_binarytree_traversePreorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_binarytree_traversePreorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePreorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePreorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePreorderWhenLess(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traverseInorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_binarytree_traverseInorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traverseInorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traverseInorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traverseInorderWhenLess(cc_binarytree *tree, cc_object *key,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_binarytree_traversePostorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_binarytree_traversePostorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePostorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePostorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
void cc_binarytree_traversePostorderWhenLess(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object));
int cc_binarytree_contain(cc_binarytree *tree, cc_object *key);
cc_object *cc_binarytree_get(cc_binarytree *tree, cc_object *key);
cc_object *cc_binarytree_getWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key);
cc_object *cc_binarytree_getWhenGreater(cc_binarytree *tree, cc_object *key);
cc_object *cc_binarytree_getWhenGreatest(cc_binarytree *tree);
cc_object *cc_binarytree_getWhenLessEqual(cc_binarytree *tree, cc_object *key);
cc_object *cc_binarytree_getWhenLess(cc_binarytree *tree, cc_object *key);
cc_object *cc_binarytree_getWhenLeast(cc_binarytree *tree);
char *cc_binarytree_tocstring(cc_binarytree *tree);

#endif /* CC_BINARYTREE_H_ */
