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
 * Author: Kentaro Aoki
 */

#include "cco_redblacktree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_redblacktree)
;
cco_redblacktree_node g_cco_redblacktree_node_nil =
{ NULL, NULL, NULL, NULL, CCO_REDBLACKTREE_COLOR_BLACK, NULL};

cco_redblacktree_status cco_redblacktree_insert_collision(cco_redblacktree_node *current_node, cco_redblacktree_node *insert_node);
cco_redblacktree_status cco_redblacktree_insert_collisionWithReplace(cco_redblacktree_node *cursor_node, cco_redblacktree_node *insert_node);
cco_redblacktree_status cco_redblacktree_insert_do(cco_redblacktree *tree, cco_v *key, cco *value, cco_redblacktree_status( cb_collision)(cco_redblacktree_node *current_node,
		cco_redblacktree_node *insert_node));
void cco_redblacktree_insert_fixup(cco_redblacktree *tree, cco_redblacktree_node *node);
void cco_redblacktree_delete_fixup(cco_redblacktree *tree, cco_redblacktree_node *node);

cco_redblacktree *cco_redblacktree_baseNew(int size)
{
	cco_redblacktree *o = NULL;
	do
	{
		if (size < sizeof(cco_redblacktree))
		{
			break;
		}
		o = (cco_redblacktree *) cco_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_redblacktree);
		cco_redblacktree_baseInitialize(o);
	} while (0);
	return o;
}

void cco_redblacktree_baseRelease(void *o)
{
	cco_redblacktree_baseFinalize(o);
	cco_baseRelease(o);
}

void cco_redblacktree_baseInitialize(cco_redblacktree *o)
{
	o->baseRelease = &cco_redblacktree_baseRelease;
	o->redblacktreeRoot = &g_cco_redblacktree_node_nil;
	return;
}

void cco_redblacktree_baseFinalize(cco_redblacktree *o)
{
	return;
}

cco_redblacktree *cco_redblacktree_new()
{
	return cco_redblacktree_baseNew(sizeof(cco_redblacktree));
}

void cco_redblacktree_release(void *o)
{
	cco_release(o);
}

cco_redblacktree_node *cco_redblacktree_node_new(void *key_cco_v, void *object_cco)
{
	cco_redblacktree_node *node = NULL;
	do
	{
		if (key_cco_v == NULL)
		{
			break;
		}
		node = malloc(sizeof(cco_redblacktree_node));
		if (node == NULL)
		{
			break;
		}
		cco_grab(key_cco_v);
		if (object_cco != NULL)
		{
			cco_grab(object_cco);
		}
		/* sets values */
		node->key = key_cco_v;
		node->object = object_cco;
		node->parent_node = &g_cco_redblacktree_node_nil;
		node->left_node = &g_cco_redblacktree_node_nil;
		node->right_node = &g_cco_redblacktree_node_nil;
		node->color = CCO_REDBLACKTREE_COLOR_RED;
	} while (0);
	return node;
}

void cco_redblacktree_node_dispose(cco_redblacktree_node *node)
{
	cco_release(node->key);
	cco_release(node->object);
	free(node);
	return;
}

cco_v *cco_redblacktree_node_getKey(cco_redblacktree_node *node)
{
	cco_v *key;

	key = node->key;
	if (key != NULL)
	{
		cco_grab(key);
	}
	return key;
}

cco *cco_redblacktree_node_getObject(cco_redblacktree_node *node)
{
	cco *object;

	object = node->object;
	if (object != NULL)
	{
		cco_grab(object);
	}
	return object;
}

/*
 * Swaps nodes of tree.
 */
void cco_redblacktree_node_swap(cco_redblacktree_node *node1, cco_redblacktree_node *node2)
{
	cco_v *tmp_key;
	cco *tmp_object;

	tmp_key = node1->key;
	tmp_object = node1->object;
	node1->key = node2->key;
	node1->object = node2->object;
	node2->key = tmp_key;
	node2->object = tmp_object;
	return;
}

/*
 * Gets a reference of maximum-node.
 */
cco_redblacktree_node *cco_redblacktree_node_maximum(cco_redblacktree_node *node)
{
	while (node->left_node != &g_cco_redblacktree_node_nil)
	{
		node = node->left_node;
	}
	return node;
}

/*
 * Gets a reference of minimum-node.
 */
cco_redblacktree_node *cco_redblacktree_node_minimum(cco_redblacktree_node *node)
{
	while (node->right_node != &g_cco_redblacktree_node_nil)
	{
		node = node->right_node;
	}
	return node;
}

/*
 * Gets a reference of successor-node.
 */
cco_redblacktree_node *cco_redblacktree_node_successor(cco_redblacktree_node *node)
{
	cco_redblacktree_node *cursor_node;
	if (node->right_node != &g_cco_redblacktree_node_nil)
	{
		return cco_redblacktree_node_minimum(node);
	}
	cursor_node = node->parent_node;
	while (cursor_node != &g_cco_redblacktree_node_nil && node == cursor_node->right_node)
	{
		node = cursor_node;
		cursor_node = cursor_node->parent_node;
	}
	return cursor_node;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with the root node:
 *  1. Visit the root.
 *  2. Traverse the left subtree.
 *  3. Traverse the right subtree.
 */
void cco_redblacktree_node_traversePreorder(cco_redblacktree_node *node, void(*cb)(cco_redblacktree_node *node))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		cb(node);
		cco_redblacktree_node_traversePreorder(node->left_node, cb);
		cco_redblacktree_node_traversePreorder(node->right_node, cb);
	}
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the root node:
 * 1. Traverse the left subtree.
 * 2. Visit the root.
 * 3. Traverse the right subtree.
 */
void cco_redblacktree_node_traverseInorder(cco_redblacktree_node *node, void(*cb)(cco_redblacktree_node *node))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		cco_redblacktree_node_traverseInorder(node->left_node, cb);
		cb(node);
		cco_redblacktree_node_traverseInorder(node->right_node, cb);
	}
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with the root node:
 * 1. Traverse the left subtree.
 * 2. Traverse the right subtree.
 * 3. Visit the root.
 */
void cco_redblacktree_node_traversePostorder(cco_redblacktree_node *node, void(*cb)(cco_redblacktree_node *node))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		cco_redblacktree_node_traversePostorder(node->left_node, cb);
		cco_redblacktree_node_traversePostorder(node->right_node, cb);
		cb(node);
	}
	return;
}

/**
 * Rotates a tree in left.
 */
void cco_redblacktree_rotateLeft(cco_redblacktree *tree, cco_redblacktree_node *node)
{
	cco_redblacktree_node *cursor_node;

	cursor_node = node->right_node;
	node->right_node = cursor_node->left_node;
	if (cursor_node->left_node != &g_cco_redblacktree_node_nil)
	{
		cursor_node->left_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cco_redblacktree_node_nil)
	{
		tree->redblacktreeRoot = cursor_node;
	}
	else if (node == node->parent_node->left_node)
	{
		node->parent_node->left_node = cursor_node;
	}
	else
	{
		node->parent_node->right_node = cursor_node;
	}
	cursor_node->left_node = node;
	node->parent_node = cursor_node;
	return;
}

/**
 * Rotates a tree in right.
 */
void cco_redblacktree_rotateRight(cco_redblacktree *tree, cco_redblacktree_node *node)
{
	cco_redblacktree_node *cursor_node;

	cursor_node = node->left_node;
	node->left_node = cursor_node->right_node;
	if (cursor_node->right_node != &g_cco_redblacktree_node_nil)
	{
		cursor_node->right_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cco_redblacktree_node_nil)
	{
		tree->redblacktreeRoot = cursor_node;
	}
	else if (node == node->parent_node->right_node)
	{
		node->parent_node->right_node = cursor_node;
	}
	else
	{
		node->parent_node->left_node = cursor_node;
	}
	cursor_node->right_node = node;
	node->parent_node = cursor_node;
	return;
}

/**
 * Inserts a node to a cco_redblacktree object.
 * @tree a cco_redblacktree object.
 * @key a cco of a key.
 * @value a cco of a value.
 * @return a cco_redblacktree_status of a result.
 * If it is inserted, A result is CCO_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CCO_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CCO_REDBLACKTREE_STATUS_ERROR.
 */
cco_redblacktree_status cco_redblacktree_insert(cco_redblacktree *tree, cco_v *key, cco *value)
{
	return cco_redblacktree_insert_do(tree, key, value, cco_redblacktree_insert_collision);
}

/**
 * Inserts a node to a cco_redblacktree object.
 * @tree a cco_redblacktree object.
 * @key a cco of a key.
 * @value a cco of a value.
 * @return a cco_redblacktree_status of a result.
 * If it is inserted, A result is CCO_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CCO_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CCO_REDBLACKTREE_STATUS_ERROR.
 */
cco_redblacktree_status cco_redblacktree_insertWithReplace(cco_redblacktree *tree, cco_v *key, cco *value)
{
	return cco_redblacktree_insert_do(tree, key, value, cco_redblacktree_insert_collisionWithReplace);
}

/**
 * Inserts a node to a cco_redblacktree object.
 * @tree a cco_redblacktree object.
 * @key a cco of a key.
 * @value a cco of a value.
 * @return a cco_redblacktree_status of a result.
 * If it is inserted, A result is CCO_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CCO_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CCO_REDBLACKTREE_STATUS_ERROR.
 */
cco_redblacktree_status cco_redblacktree_insertWithCallback(cco_redblacktree *tree, cco_v *key, cco *value, cco_redblacktree_status( cb_collision)(cco *input_value, cco *target_value))
{
	return cco_redblacktree_insert_do(tree, key, value, cco_redblacktree_insert_collisionWithReplace);
}

cco_redblacktree_status cco_redblacktree_insert_collision(cco_redblacktree_node *current_node, cco_redblacktree_node *insert_node)
{
	cco_redblacktree_node_dispose(insert_node);
	return CCO_REDBLACKTREE_STATUS_FAILURE;
}

cco_redblacktree_status cco_redblacktree_insert_collisionWithReplace(cco_redblacktree_node *cursor_node, cco_redblacktree_node *insert_node)
{
	/* replaces a node to this tree. */
	cco_redblacktree_node_swap(cursor_node, insert_node);
	cco_redblacktree_node_dispose(insert_node);
	return CCO_REDBLACKTREE_STATUS_REPLACED;
}

cco_redblacktree_status cco_redblacktree_insert_do(cco_redblacktree *tree, cco_v *key, cco *value, cco_redblacktree_status( cb_collision)(cco_redblacktree_node *current_node,
		cco_redblacktree_node *insert_node))
{
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_INSERTED;
	cco_redblacktree_node *insert_node;
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_node *leaf_node;
	int insert_way = 0;

	do
	{
		if (tree == NULL || key == NULL || value == NULL)
		{
			result = CCO_REDBLACKTREE_STATUS_ERROR;
			break;
		}
		/* creates a node to insert an object. */
		insert_node = cco_redblacktree_node_new(key, value);
		/* moves a cursor to leaf. */
		leaf_node = &g_cco_redblacktree_node_nil;
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			leaf_node = cursor_node;
			insert_way = key->v_compere(key, cursor_node->key);
			if (insert_way == 0)
			{
				break;
			}
			else if (insert_way < 0)
			{
				cursor_node = cursor_node->left_node;
			}
			else
			{
				cursor_node = cursor_node->right_node;
			}
		}
		if (cursor_node == &g_cco_redblacktree_node_nil)
		{
			/* connects a node to this tree. */
			insert_node->parent_node = leaf_node;
			if (leaf_node == &g_cco_redblacktree_node_nil)
			{
				tree->redblacktreeRoot = insert_node;
			}
			else if (insert_way < 0)
			{
				leaf_node->left_node = insert_node;
			}
			else
			{
				leaf_node->right_node = insert_node;
			}
			/* fixes up for data structure. */
			cco_redblacktree_insert_fixup(tree, insert_node);
		}
		else
		{
			result = (cco_redblacktree_status) cb_collision(cursor_node, insert_node);
		}
	} while (0);
	return result;
}

/**
 * Fixes up a tree for the cco_redblacktree_insert.
 */
void cco_redblacktree_insert_fixup(cco_redblacktree *tree, cco_redblacktree_node *node)
{
	cco_redblacktree_node *cursor_z_node = node;
	cco_redblacktree_node *cursor_y_node;

	while (cursor_z_node->parent_node->color == CCO_REDBLACKTREE_COLOR_RED)
	{
		if (cursor_z_node->parent_node == cursor_z_node->parent_node->parent_node->left_node)
		{
			cursor_y_node = cursor_z_node->parent_node->parent_node->right_node;
			if (cursor_y_node->color == CCO_REDBLACKTREE_COLOR_RED)
			{
				cursor_z_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_y_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				/* updates cursor_z_node */
				cursor_z_node = cursor_z_node->parent_node->parent_node;
			}
			else
			{
				if (cursor_z_node == cursor_z_node->parent_node->right_node)
				{
					/* updates cursor_z_node */
					cursor_z_node = cursor_z_node->parent_node;
					/* left-rotate cursor_z_node */
					cco_redblacktree_rotateLeft(tree, cursor_z_node);
				}
				cursor_z_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				/* right-rotate cursor_z_node->parent_node->parent_node */
				cco_redblacktree_rotateRight(tree, cursor_z_node->parent_node->parent_node);
			}
		}
		else
		{
			cursor_y_node = cursor_z_node->parent_node->parent_node->left_node;
			if (cursor_y_node->color == CCO_REDBLACKTREE_COLOR_RED)
			{
				cursor_z_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_y_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				/* updates cursor_z_node */
				cursor_z_node = cursor_z_node->parent_node->parent_node;
			}
			else
			{
				if (cursor_z_node == cursor_z_node->parent_node->left_node)
				{
					/* updates cursor_z_node */
					cursor_z_node = cursor_z_node->parent_node;
					/* right-rotate cursor_z_node */
					cco_redblacktree_rotateRight(tree, cursor_z_node);
				}
				cursor_z_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				/* left-rotate cursor_z_node->parent_node->parent_node */
				cco_redblacktree_rotateLeft(tree, cursor_z_node->parent_node->parent_node);
			}
		}
	}
	tree->redblacktreeRoot->color = CCO_REDBLACKTREE_COLOR_BLACK;
	return;
}

/**
 * Deletes a node from a tree.
 */
cco_redblacktree_status cco_redblacktree_delete(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *delete_node;
	cco_redblacktree_node *cursor_x_node;
	cco_redblacktree_node *cursor_y_node;
	int find_way;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;

	do
	{
		if (tree == NULL || key == NULL)
		{
			result = CCO_REDBLACKTREE_STATUS_ERROR;
			break;
		}
		/* finds a node to delete it from a tree. */
		delete_node = &g_cco_redblacktree_node_nil;
		cursor_y_node = tree->redblacktreeRoot;
		while (cursor_y_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_y_node->key);
			if (find_way == 0)
			{
				delete_node = cursor_y_node;
				break;
			}
			else if (find_way < 0)
			{
				cursor_y_node = cursor_y_node->left_node;
			}
			else
			{
				cursor_y_node = cursor_y_node->right_node;
			}
		}
		/* deletes a node from a tree. */
		if (delete_node == &g_cco_redblacktree_node_nil)
		{
			result = CCO_REDBLACKTREE_STATUS_FAILURE;
			break;
		}
		if (delete_node->left_node == &g_cco_redblacktree_node_nil || delete_node->right_node == &g_cco_redblacktree_node_nil)
		{
			cursor_y_node = delete_node;
		}
		else
		{
			cursor_y_node = cco_redblacktree_node_successor(delete_node);
		}
		if (cursor_y_node->left_node != &g_cco_redblacktree_node_nil)
		{
			cursor_x_node = cursor_y_node->left_node;
		}
		else
		{
			cursor_x_node = cursor_y_node->right_node;
		}
		cursor_x_node->parent_node = cursor_y_node->parent_node;
		if (cursor_y_node->parent_node == &g_cco_redblacktree_node_nil)
		{
			tree->redblacktreeRoot = cursor_x_node;
		}
		else
		{
			if (cursor_y_node == cursor_y_node->parent_node->left_node)
			{
				cursor_y_node->parent_node->left_node = cursor_x_node;
			}
			else
			{
				cursor_y_node->parent_node->right_node = cursor_x_node;
			}
		}
		if (cursor_y_node != delete_node)
		{
			cco_redblacktree_node_swap(delete_node, cursor_y_node);

		}
		if (cursor_y_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
		{
			cco_redblacktree_delete_fixup(tree, cursor_x_node);
		}
		cco_redblacktree_node_dispose(cursor_y_node);
	} while (0);
	return result;
}

/**
 * Fixes up a tree for the cco_redblacktree_delete.
 */
void cco_redblacktree_delete_fixup(cco_redblacktree *tree, cco_redblacktree_node *node)
{
	cco_redblacktree_node *cursor_x_node = node;
	cco_redblacktree_node *cursor_w_node;

	while (cursor_x_node != tree->redblacktreeRoot && cursor_x_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
	{
		if (cursor_x_node == cursor_x_node->parent_node->left_node)
		{
			cursor_w_node = cursor_x_node->parent_node->right_node;
			if (cursor_w_node->color == CCO_REDBLACKTREE_COLOR_RED)
			{
				cursor_w_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_x_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				cco_redblacktree_rotateLeft(tree, cursor_x_node->parent_node);
				cursor_w_node = cursor_x_node->parent_node->right_node;
			}
			if (cursor_w_node->left_node->color == CCO_REDBLACKTREE_COLOR_BLACK && cursor_w_node->right_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
			{
				cursor_w_node->color = CCO_REDBLACKTREE_COLOR_RED;
				cursor_x_node = cursor_x_node->parent_node;
			}
			else
			{
				if (cursor_w_node->right_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
				{
					cursor_w_node->left_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
					cursor_w_node->color = CCO_REDBLACKTREE_COLOR_RED;
					cco_redblacktree_rotateRight(tree, cursor_w_node);
					cursor_w_node = cursor_x_node->parent_node->right_node;
				}
				cursor_w_node->color = cursor_x_node->parent_node->color;
				cursor_x_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_w_node->right_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cco_redblacktree_rotateLeft(tree, cursor_x_node->parent_node);
				cursor_x_node = tree->redblacktreeRoot;
			}
		}
		else
		{
			cursor_w_node = cursor_x_node->parent_node->left_node;
			if (cursor_w_node->color == CCO_REDBLACKTREE_COLOR_RED)
			{
				cursor_w_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_x_node->parent_node->color = CCO_REDBLACKTREE_COLOR_RED;
				cco_redblacktree_rotateRight(tree, cursor_x_node->parent_node);
				cursor_w_node = cursor_x_node->parent_node->left_node;
			}
			if (cursor_w_node->right_node->color == CCO_REDBLACKTREE_COLOR_BLACK && cursor_w_node->left_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
			{
				cursor_w_node->color = CCO_REDBLACKTREE_COLOR_RED;
				cursor_x_node = cursor_x_node->parent_node;
			}
			else
			{
				if (cursor_w_node->left_node->color == CCO_REDBLACKTREE_COLOR_BLACK)
				{
					cursor_w_node->right_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
					cursor_w_node->color = CCO_REDBLACKTREE_COLOR_RED;
					cco_redblacktree_rotateLeft(tree, cursor_w_node);
					cursor_w_node = cursor_x_node->parent_node->left_node;
				}
				cursor_w_node->color = cursor_x_node->parent_node->color;
				cursor_x_node->parent_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cursor_w_node->left_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
				cco_redblacktree_rotateRight(tree, cursor_x_node->parent_node);
				cursor_x_node = tree->redblacktreeRoot;
			}
		}
	}
	cursor_x_node->color = CCO_REDBLACKTREE_COLOR_BLACK;
	return;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenEqual(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_node->key);
			if (find_way == 0)
			{
				break;
			}
			else if (find_way < 0)
			{
				cursor_node = cursor_node->left_node;
			}
			else
			{
				cursor_node = cursor_node->right_node;
			}
		}
	} while (0);
	return cursor_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenGreaterEqual(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_node->key);
			if (find_way == 0)
			{
				break;
			}
			else if (find_way < 0)
			{
				break;
			}
			else
			{
				cursor_node = cursor_node->right_node;
			}
		}
	} while (0);
	return cursor_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenGreater(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_node->key);
			if (find_way == 0)
			{
				cursor_node = cursor_node->right_node;
			}
			else if (find_way < 0)
			{
				break;
			}
			else
			{
				cursor_node = cursor_node->right_node;
			}
		}
	} while (0);
	return cursor_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenGreatest(cco_redblacktree *tree)
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_node *target_node;

	do
	{
		if (tree == NULL)
		{
			target_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		target_node = &g_cco_redblacktree_node_nil;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			target_node = cursor_node;
			cursor_node = cursor_node->right_node;
		}
	} while (0);
	return target_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenLessEqual(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_node->key);
			if (find_way == 0)
			{
				break;
			}
			else if (find_way < 0)
			{
				cursor_node = cursor_node->left_node;
			}
			else
			{
				break;
			}
		}
	} while (0);
	return cursor_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenLess(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			find_way = key->v_compere(key, cursor_node->key);
			if (find_way == 0)
			{
				cursor_node = cursor_node->left_node;
			}
			else if (find_way < 0)
			{
				cursor_node = cursor_node->left_node;
			}
			else
			{
				break;
			}
		}
	} while (0);
	return cursor_node;
}

cco_redblacktree_node *cco_redblacktree_findNodeWhenLeast(cco_redblacktree *tree)
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_node *target_node;

	do
	{
		if (tree == NULL)
		{
			target_node = NULL;
			break;
		}
		cursor_node = tree->redblacktreeRoot;
		target_node = &g_cco_redblacktree_node_nil;
		while (cursor_node != &g_cco_redblacktree_node_nil)
		{
			target_node = cursor_node;
			cursor_node = cursor_node->left_node;
		}
	} while (0);
	return target_node;
}

/*
 * Calls a callback to the node.
 */
void cco_redblacktree_traverse_preorder(cco_redblacktree_node *node, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		callback(callbackobject, node->key, node->object);
		cco_redblacktree_traverse_preorder(node->left_node, callbackobject, callback);
		cco_redblacktree_traverse_preorder(node->right_node, callbackobject, callback);
	}
	return;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with the root node:
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePreorder(cco_redblacktree *tree, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cco_redblacktree_traverse_preorder(tree->redblacktreeRoot, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePreorderWhenGreaterEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cco_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePreorderWhenGreater(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreater(tree, key);
		cco_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePreorderWhenLessEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLessEqual(tree, key);
		cco_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePreorderWhenLess(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLess(tree, key);
		cco_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/*
 * Calls a callback to the node.
 */
void cco_redblacktree_traverse_inorder(cco_redblacktree_node *node, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		cco_redblacktree_traverse_inorder(node->left_node, callbackobject, callback);
		callback(callbackobject, node->key, node->object);
		cco_redblacktree_traverse_inorder(node->right_node, callbackobject, callback);
	}
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the root node.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traverseInorder(cco_redblacktree *tree, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cco_redblacktree_traverse_inorder(tree->redblacktreeRoot, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traverseIneorderWhenGreaterEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cco_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traverseIneorderWhenGreater(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreater(tree, key);
		cco_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traverseIneorderWhenLessEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLessEqual(tree, key);
		cco_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traverseIneorderWhenLess(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLess(tree, key);
		cco_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/*
 * Calls a callback to the node.
 */
void cco_redblacktree_traverse_postorder(cco_redblacktree_node *node, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	if (node != &g_cco_redblacktree_node_nil)
	{
		cco_redblacktree_traverse_postorder(node->left_node, callbackobject, callback);
		cco_redblacktree_traverse_postorder(node->right_node, callbackobject, callback);
		callback(callbackobject, node->key, node->object);
	}
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with the root node.
 * 1. Traverse the left subtree.
 * 2. Visit the root node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePostorder(cco_redblacktree *tree, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cco_redblacktree_traverse_postorder(tree->redblacktreeRoot, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePostorderWhenGreaterEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cco_redblacktree_traverse_postorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePostorderWhenGreater(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenGreater(tree, key);
		cco_redblacktree_traverse_postorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePostorderWhenLessEqual(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLessEqual(tree, key);
		cco_redblacktree_traverse_postorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
cco_redblacktree_status cco_redblacktree_traversePostorderWhenLess(cco_redblacktree *tree, cco_v *key, cco *callbackobject, void(*callback)(cco *callbackobject, cco_v *key, cco *object))
{
	cco_redblacktree_node *cursor_node;
	cco_redblacktree_status result = CCO_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CCO_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cco_redblacktree_findNodeWhenLess(tree, key);
		cco_redblacktree_traverse_postorder(cursor_node, callbackobject, callback);
	}
	return result;
}

int cco_redblacktree_contain(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	int result = 0;
	if (tree != NULL && key != NULL)
	{
		cursor_node = cco_redblacktree_findNodeWhenEqual(tree, key);
		if (cursor_node != &g_cco_redblacktree_node_nil)
		{
			result = 1;
		}
	}
	return result;
}

cco *cco_redblacktree_get(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenEqual(tree, key);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenGreaterEqual(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenGreaterEqual(tree, key);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenGreater(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenGreater(tree, key);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenGreatest(cco_redblacktree *tree)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenGreatest(tree);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenLessEqual(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenLessEqual(tree, key);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenLess(cco_redblacktree *tree, cco_v *key)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenLess(tree, key);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cco *cco_redblacktree_getWhenLeast(cco_redblacktree *tree)
{
	cco_redblacktree_node *cursor_node;
	cco *result_object = NULL;
	cursor_node = cco_redblacktree_findNodeWhenLeast(tree);
	if (cursor_node != &g_cco_redblacktree_node_nil)
	{
		result_object = cco_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

#if 0
void cco_redblacktree_tocstring_sub(cco *callbackobject, cco *key, cco *object)
{
	char *key_cstr;
	char *obj_cstr;
	cc_string *key_str;
	cc_string *obj_str;
	cc_string *str1;
	cc_string *str2;
	cc_string *str3;

	str1 = cc_string_new("<ReadBlackTree key=\"");
	str2 = cc_string_new("\">\n");
	str3 = cc_string_new("</ReadBlackTree>\n");

	key_cstr = key->tocstring(key);
	obj_cstr = object->tocstring(object);
	key_str = cc_string_new(key_cstr);
	obj_str = cc_string_new(obj_cstr);
	cc_string_catenate(callbackobject, str1);
	cc_string_catenate(callbackobject, key_str);
	cc_string_catenate(callbackobject, str2);
	cc_string_catenate(callbackobject, obj_str);
	cc_string_catenate(callbackobject, str3);
	cco_release(key_str);
	cco_release(obj_str);
	free(key_cstr);
	free(obj_cstr);

	cco_release(str1);
	cco_release(str2);
	cco_release(str3);

	return;
}

char *cco_redblacktree_tocstring(cco_redblacktree *table)
{
	cc_string *str;
	char *cstring = NULL;

	str = cc_string_new("");
	cco_redblacktree_traversePreorder(table, str, cco_redblacktree_tocstring_sub);
	cstring = str->tocstring(str);
	cco_release(str);
	return cstring;
}

/*
 * For debug, prints a tree to the stdout.
 */
void cco_redblacktree_printNode(cco_redblacktree *tree,
		cco_redblacktree_node *node, int depth)
{
	int i;
	char *cs_node_key;

	if (node != &g_cco_redblacktree_node_nil)
	{
		cco_redblacktree_printNode(tree, node->left_node, depth + 1);
		for (i = 0; i < depth; i++)
		{
			printf("*");
		}
		cs_node_key = node->key->tocstring(node->key);
		printf("%s\n", cs_node_key);
		free(cs_node_key);
		cco_redblacktree_printNode(tree, node->right_node, depth + 1);
	}

}

#endif
