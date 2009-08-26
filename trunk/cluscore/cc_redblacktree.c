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

#include "cc_redblacktree.h"
#include "cc_string.h"
#include "cc_container.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* printf */

/*
 * Defines prototypes and Structures that it is private.
 */
typedef struct cc_redblacktree_properties cc_redblacktree_properties;
typedef struct cc_redblacktree_node cc_redblacktree_node;
typedef enum cc_redblacktree_color cc_redblacktree_color;

/** Defines a enum of a color for the cc_redblacktree. */
enum cc_redblacktree_color
{
	CC_REDBLACKTREE_COLOR_RED, CC_REDBLACKTREE_COLOR_BLACK
};

/** Defines a struct of a node for the cc_redblacktree. */
struct cc_redblacktree_node
{
	cc_object *key;
	cc_redblacktree_node *left_node;
	cc_redblacktree_node *right_node;
	cc_redblacktree_node *parent_node;
	cc_redblacktree_color color;
	cc_object *object;
};

/** Defines a properties of a object for the cc_redblacktree. */
struct cc_redblacktree_properties
{
	cc_redblacktree_node *root_node;
	cc_redblacktree_node *cursor_node;
};

/* Defines prototypes of the cc_redblacktree for private. */
cc_redblacktree_properties *cc_redblacktree_properties_new();
void cc_redblacktree_properties_dispose(cc_redblacktree_properties *properties);
inline cc_redblacktree_node *cc_redblacktree_node_new(cc_object *key,
		cc_object *object);
void cc_redblacktree_node_dispose(cc_redblacktree_node *node);
void cc_redblacktree_node_swap(cc_redblacktree_node *node1,
		cc_redblacktree_node *node2);
void cc_redblacktree_node_traversePreorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node));
void cc_redblacktree_node_traverseInorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node));
void cc_redblacktree_node_traversePostorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node));
cc_redblacktree_status cc_redblacktree_insert_collision(
		cc_redblacktree_node *current_node, cc_redblacktree_node *insert_node);
cc_redblacktree_status cc_redblacktree_insert_collisionWithReplace(
		cc_redblacktree_node *current_node, cc_redblacktree_node *insert_node);
cc_redblacktree_status cc_redblacktree_insert_do(cc_redblacktree *tree,
		cc_object *key, cc_object *value,
		cc_redblacktree_status( cb_collision)(
				cc_redblacktree_node *current_node,
				cc_redblacktree_node *insert_node));
void cc_redblacktree_insert_fixup(cc_redblacktree *tree,
		cc_redblacktree_node *insert_node);
void cc_redblacktree_delete_fixup(cc_redblacktree *tree,
		cc_redblacktree_node *cursor_x_node);
void cc_redblacktree_traverse_preorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_redblacktree_traverse_inorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_redblacktree_traverse_postorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object));
void cc_redblacktree_printNode(cc_redblacktree *tree,
		cc_redblacktree_node *node, int depth);

/** Defines the Nil-Node for cc_redblacktree. */
cc_redblacktree_node g_cc_redblacktree_node_nil =
{ NULL, NULL, NULL, NULL, CC_REDBLACKTREE_COLOR_BLACK, NULL};

/** Defines the Object-ID of redblacktree. */
int g_cc_redblacktree_object_id;

/**
 * Allocates a properties of the cc_redblacktree when dealt by cc_redblacktree_new().
 * @return a properties of cc_redblacktree.
 */
cc_redblacktree_properties *cc_redblacktree_properties_new()
{
	cc_redblacktree_properties *properties = NULL;
	/* creates cc_redblacktree_properties */
	properties = malloc(sizeof(cc_redblacktree_properties));
	if (properties != NULL)
	{
		/* sets values */
		properties->root_node = &g_cc_redblacktree_node_nil;
	}
	return properties;
}

/**
 * Frees a properties of the cc_redblacktree when dealt by cc_redblacktree_release().
 * @properties a properties of cc_redblacktree to free.
 */
void cc_redblacktree_properties_dispose(cc_redblacktree_properties *properties)
{
	if (properties != NULL)
	{
		/* disposes cc_redblacktree_properties */
		cc_redblacktree_node_traversePostorder(properties->root_node,
				&cc_redblacktree_node_dispose);
		free(properties);
	}
	return;
}

/**
 * Creates a object of the cc_redblacktree.
 * @return a object of the cc_redblacktree.
 */
cc_redblacktree *cc_redblacktree_new()
{
	cc_redblacktree *object = NULL;
	cc_redblacktree_properties *properties = cc_redblacktree_properties_new();
	object = cc_object_new(&g_cc_redblacktree_object_id, properties,
			cc_redblacktree_properties_dispose);
	/* sets call-back functions. */
	object->tocstring = (void *) cc_redblacktree_tocstring;
	return (object);
}

/*
 * Releases a object of the cc_redblacktree.
 * @tree a object of cc_redblacktree to free.
 */
void cc_redblacktree_release(cc_redblacktree *tree)
{
	cc_object_release(tree);
	return;
}

/*
 * Allocates a node of tree.
 */
inline cc_redblacktree_node *cc_redblacktree_node_new(cc_object *key,
		cc_object *object)
{
	cc_redblacktree_node *node = NULL;
	do
	{
		if (key == NULL)
		{
			break;
		}
		node = malloc(sizeof(cc_redblacktree_node));
		if (node == NULL)
		{
			break;
		}
		cc_object_grab(key);
		if (object != NULL)
		{
			cc_object_grab(object);
		}
		/* sets values */
		node->key = key;
		node->object = object;
		node->parent_node = &g_cc_redblacktree_node_nil;
		node->left_node = &g_cc_redblacktree_node_nil;
		node->right_node = &g_cc_redblacktree_node_nil;
		node->color = CC_REDBLACKTREE_COLOR_RED;
	} while (0);
	return node;
}

/*
 * Frees a node of tree.
 */
void cc_redblacktree_node_dispose(cc_redblacktree_node *node)
{
	cc_object_release(node->key);
	cc_object_release(node->object);
	free(node);
	return;
}

/*
 * Gets a key object from a node.
 */
cc_object *cc_redblacktree_node_getKey(cc_redblacktree_node *node)
{
	cc_object *key;

	key = node->key;
	if (key != NULL)
	{
		cc_object_grab(key);
	}
	return key;
}

/*
 * Gets a object from a node.
 */
cc_object *cc_redblacktree_node_getObject(cc_redblacktree_node *node)
{
	cc_object *object;

	object = node->object;
	if (object != NULL)
	{
		cc_object_grab(object);
	}
	return object;
}

/*
 * Gets a c-stype string of a node.
 */
char *cc_redblacktree_node_tocstring(cc_redblacktree_node *node)
{
	cc_string *string_base;
	cc_string *string_close;
	cc_string *string_key;
	cc_string *string_object;
	char *cstring;
	char *result = NULL;

	if (node != NULL)
	{
		string_base = cc_string_new("[");
		string_close = cc_string_new("]");
		cstring = node->key->tocstring(node->key);
		string_key = cc_string_new(cstring);
		free(cstring);
		cstring = node->object->tocstring(node->object);
		string_object = cc_string_new(cstring);
		free(cstring);
		cc_string_catenate(string_base, string_key);
		cc_string_catenate(string_base, string_close);
		cc_string_catenate(string_base, string_object);
		result = string_base->tocstring(string_base);
		cc_string_release(string_base);
		cc_string_release(string_close);
		cc_string_release(string_key);
		cc_string_release(string_object);
	}
	return result;
}

/*
 * Swaps a node of tree.
 */
void cc_redblacktree_node_swap(cc_redblacktree_node *node1,
		cc_redblacktree_node *node2)
{
	cc_object *tmp_key;
	cc_object *tmp_object;

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
cc_redblacktree_node *cc_redblacktree_node_maximum(cc_redblacktree_node *node)
{
	while (node->left_node != &g_cc_redblacktree_node_nil)
	{
		node = node->left_node;
	}
	return node;
}

/*
 * Gets a reference of minimum-node.
 */
cc_redblacktree_node *cc_redblacktree_node_minimum(cc_redblacktree_node *node)
{
	while (node->right_node != &g_cc_redblacktree_node_nil)
	{
		node = node->right_node;
	}
	return node;
}

/*
 * Gets a reference of successor-node.
 */
cc_redblacktree_node *cc_redblacktree_node_successor(cc_redblacktree_node *node)
{
	cc_redblacktree_node *cursor_node;
	if (node->right_node != &g_cc_redblacktree_node_nil)
	{
		return cc_redblacktree_node_minimum(node);
	}
	cursor_node = node->parent_node;
	while (cursor_node != &g_cc_redblacktree_node_nil && node
			== cursor_node->right_node)
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
void cc_redblacktree_node_traversePreorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		cb(node);
		cc_redblacktree_node_traversePreorder(node->left_node, cb);
		cc_redblacktree_node_traversePreorder(node->right_node, cb);
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
void cc_redblacktree_node_traverseInorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		cc_redblacktree_node_traverseInorder(node->left_node, cb);
		cb(node);
		cc_redblacktree_node_traverseInorder(node->right_node, cb);
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
void cc_redblacktree_node_traversePostorder(cc_redblacktree_node *node,
		void(*cb)(cc_redblacktree_node *node))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		cc_redblacktree_node_traversePostorder(node->left_node, cb);
		cc_redblacktree_node_traversePostorder(node->right_node, cb);
		cb(node);
	}
	return;
}

/**
 * Rotates a tree in left.
 */
inline void cc_redblacktree_rotateLeft(cc_redblacktree *tree,
		cc_redblacktree_node *node)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;

	properties = tree->properties;
	cursor_node = node->right_node;
	node->right_node = cursor_node->left_node;
	if (cursor_node->left_node != &g_cc_redblacktree_node_nil)
	{
		cursor_node->left_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cc_redblacktree_node_nil)
	{
		properties->root_node = cursor_node;
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
inline void cc_redblacktree_rotateRight(cc_redblacktree *tree,
		cc_redblacktree_node *node)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;

	properties = tree->properties;
	cursor_node = node->left_node;
	node->left_node = cursor_node->right_node;
	if (cursor_node->right_node != &g_cc_redblacktree_node_nil)
	{
		cursor_node->right_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cc_redblacktree_node_nil)
	{
		properties->root_node = cursor_node;
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
 * Inserts a node to a cc_redblacktree object.
 * @tree a cc_redblecktree object.
 * @key a cc_object of a key.
 * @value a cc_object of a value.
 * @return a cc_redblacktree_status of a result.
 * If it is inserted, A result is CC_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CC_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CC_REDBLACKTREE_STATUS_ERROR.
 */
cc_redblacktree_status cc_redblacktree_insert(cc_redblacktree *tree,
		cc_object *key, cc_object *value)
{
	return cc_redblacktree_insert_do(tree, key, value,
			cc_redblacktree_insert_collision);
}

/**
 * Inserts a node to a cc_redblacktree object.
 * @tree a cc_redblecktree object.
 * @key a cc_object of a key.
 * @value a cc_object of a value.
 * @return a cc_redblacktree_status of a result.
 * If it is inserted, A result is CC_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CC_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CC_REDBLACKTREE_STATUS_ERROR.
 */
cc_redblacktree_status cc_redblacktree_insertWithReplace(cc_redblacktree *tree,
		cc_object *key, cc_object *value)
{
	return cc_redblacktree_insert_do(tree, key, value,
			cc_redblacktree_insert_collisionWithReplace);
}

/**
 * Inserts a node to a cc_redblacktree object.
 * @tree a cc_redblecktree object.
 * @key a cc_object of a key.
 * @value a cc_object of a value.
 * @return a cc_redblacktree_status of a result.
 * If it is inserted, A result is CC_REDBLACKTREE_STATUS_INSERTED.
 * If it is replaced and inserted, A result is CC_REDBLACKTREE_STATUS_REPLACED.
 * If it is error, A result is CC_REDBLACKTREE_STATUS_ERROR.
 */
cc_redblacktree_status cc_redblacktree_insertWithCallback(cc_redblacktree *tree,
		cc_object *key, cc_object *value,
		cc_redblacktree_status( cb_collision)(
				cc_object *input_value,
				cc_object *target_value))
{
	return cc_redblacktree_insert_do(tree, key, value,
			cc_redblacktree_insert_collisionWithReplace);
}

cc_redblacktree_status cc_redblacktree_insert_collision(
		cc_redblacktree_node *current_node, cc_redblacktree_node *insert_node)
{
	cc_redblacktree_node_dispose(insert_node);
	return CC_REDBLACKTREE_STATUS_FAILURE;
}

cc_redblacktree_status cc_redblacktree_insert_collisionWithReplace(
		cc_redblacktree_node *cursor_node, cc_redblacktree_node *insert_node)
{
	/* replaces a node to this tree. */
	cc_redblacktree_node_swap(cursor_node, insert_node);
	cc_redblacktree_node_dispose(insert_node);
	return CC_REDBLACKTREE_STATUS_REPLACED;
}

cc_redblacktree_status cc_redblacktree_insert_do(cc_redblacktree *tree,
		cc_object *key, cc_object *value,
		cc_redblacktree_status( cb_collision)(
				cc_redblacktree_node *current_node,
				cc_redblacktree_node *insert_node))
{
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_INSERTED;
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *insert_node;
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_node *leaf_node;
	int insert_way = 0;

	do
	{
		if (tree == NULL || key == NULL || value == NULL)
		{
			result = CC_REDBLACKTREE_STATUS_ERROR;
			break;
		}
		properties = tree->properties;
		/* creates a node to insert an object. */
		insert_node = cc_redblacktree_node_new(key, value);
		/* moves a cursor to leaf. */
		leaf_node = &g_cc_redblacktree_node_nil;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			leaf_node = cursor_node;
			insert_way = key->compare(key, cursor_node->key);
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
		if (cursor_node == &g_cc_redblacktree_node_nil)
		{
			/* connects a node to this tree. */
			insert_node->parent_node = leaf_node;
			if (leaf_node == &g_cc_redblacktree_node_nil)
			{
				properties->root_node = insert_node;
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
			cc_redblacktree_insert_fixup(tree, insert_node);
		}
		else
		{
			result = (cc_redblacktree_status) cb_collision(cursor_node,
					insert_node);
		}
	} while (0);
	return result;
}

/**
 * Fixes up a tree for the cc_redblacktree_insert.
 */
void cc_redblacktree_insert_fixup(cc_redblacktree *tree,
		cc_redblacktree_node *node)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_z_node = node;
	cc_redblacktree_node *cursor_y_node;

	properties = tree->properties;
	while (cursor_z_node->parent_node->color == CC_REDBLACKTREE_COLOR_RED)
	{
		if (cursor_z_node->parent_node
				== cursor_z_node->parent_node->parent_node->left_node)
		{
			cursor_y_node = cursor_z_node->parent_node->parent_node->right_node;
			if (cursor_y_node->color == CC_REDBLACKTREE_COLOR_RED)
			{
				cursor_z_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_y_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color
						= CC_REDBLACKTREE_COLOR_RED;
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
					cc_redblacktree_rotateLeft(tree, cursor_z_node);
				}
				cursor_z_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color
						= CC_REDBLACKTREE_COLOR_RED;
				/* right-rotate cursor_z_node->parent_node->parent_node */
				cc_redblacktree_rotateRight(tree,
						cursor_z_node->parent_node->parent_node);
			}
		}
		else
		{
			cursor_y_node = cursor_z_node->parent_node->parent_node->left_node;
			if (cursor_y_node->color == CC_REDBLACKTREE_COLOR_RED)
			{
				cursor_z_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_y_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color
						= CC_REDBLACKTREE_COLOR_RED;
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
					cc_redblacktree_rotateRight(tree, cursor_z_node);
				}
				cursor_z_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_z_node->parent_node->parent_node->color
						= CC_REDBLACKTREE_COLOR_RED;
				/* left-rotate cursor_z_node->parent_node->parent_node */
				cc_redblacktree_rotateLeft(tree,
						cursor_z_node->parent_node->parent_node);
			}
		}
	}
	properties->root_node->color = CC_REDBLACKTREE_COLOR_BLACK;
	return;
}

/**
 * Deletes a node from a tree.
 */
cc_redblacktree_status cc_redblacktree_delete(cc_redblacktree *tree,
		cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *delete_node;
	cc_redblacktree_node *cursor_x_node;
	cc_redblacktree_node *cursor_y_node;
	int find_way;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	do
	{
		if (tree == NULL || key == NULL)
		{
			result = CC_REDBLACKTREE_STATUS_ERROR;
			break;
		}
		properties = tree->properties;
		/* finds a node to delete it from a tree. */
		delete_node = &g_cc_redblacktree_node_nil;
		cursor_y_node = properties->root_node;
		while (cursor_y_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_y_node->key);
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
		if (delete_node == &g_cc_redblacktree_node_nil)
		{
			result = CC_REDBLACKTREE_STATUS_FAILURE;
			break;
		}
		if (delete_node->left_node == &g_cc_redblacktree_node_nil
				|| delete_node->right_node == &g_cc_redblacktree_node_nil)
		{
			cursor_y_node = delete_node;
		}
		else
		{
			cursor_y_node = cc_redblacktree_node_successor(delete_node);
		}
		if (cursor_y_node->left_node != &g_cc_redblacktree_node_nil)
		{
			cursor_x_node = cursor_y_node->left_node;
		}
		else
		{
			cursor_x_node = cursor_y_node->right_node;
		}
		cursor_x_node->parent_node = cursor_y_node->parent_node;
		if (cursor_y_node->parent_node == &g_cc_redblacktree_node_nil)
		{
			properties->root_node = cursor_x_node;
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
			cc_redblacktree_node_swap(delete_node, cursor_y_node);

		}
		if (cursor_y_node->color == CC_REDBLACKTREE_COLOR_BLACK)
		{
			cc_redblacktree_delete_fixup(tree, cursor_x_node);
		}
		cc_redblacktree_node_dispose(cursor_y_node);
	} while (0);
	return result;
}

/**
 * Fixes up a tree for the cc_redblacktree_delete.
 */
void cc_redblacktree_delete_fixup(cc_redblacktree *tree,
		cc_redblacktree_node *node)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_x_node = node;
	cc_redblacktree_node *cursor_w_node;

	properties = tree->properties;
	while (cursor_x_node != properties->root_node && cursor_x_node->color
			== CC_REDBLACKTREE_COLOR_BLACK)
	{
		if (cursor_x_node == cursor_x_node->parent_node->left_node)
		{
			cursor_w_node = cursor_x_node->parent_node->right_node;
			if (cursor_w_node->color == CC_REDBLACKTREE_COLOR_RED)
			{
				cursor_w_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_x_node->parent_node->color = CC_REDBLACKTREE_COLOR_RED;
				cc_redblacktree_rotateLeft(tree, cursor_x_node->parent_node);
				cursor_w_node = cursor_x_node->parent_node->right_node;
			}
			if (cursor_w_node->left_node->color == CC_REDBLACKTREE_COLOR_BLACK
					&& cursor_w_node->right_node->color
							== CC_REDBLACKTREE_COLOR_BLACK)
			{
				cursor_w_node->color = CC_REDBLACKTREE_COLOR_RED;
				cursor_x_node = cursor_x_node->parent_node;
			}
			else
			{
				if (cursor_w_node->right_node->color
						== CC_REDBLACKTREE_COLOR_BLACK)
				{
					cursor_w_node->left_node->color
							= CC_REDBLACKTREE_COLOR_BLACK;
					cursor_w_node->color = CC_REDBLACKTREE_COLOR_RED;
					cc_redblacktree_rotateRight(tree, cursor_w_node);
					cursor_w_node = cursor_x_node->parent_node->right_node;
				}
				cursor_w_node->color = cursor_x_node->parent_node->color;
				cursor_x_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_w_node->right_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cc_redblacktree_rotateLeft(tree, cursor_x_node->parent_node);
				cursor_x_node = properties->root_node;
			}
		}
		else
		{
			cursor_w_node = cursor_x_node->parent_node->left_node;
			if (cursor_w_node->color == CC_REDBLACKTREE_COLOR_RED)
			{
				cursor_w_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_x_node->parent_node->color = CC_REDBLACKTREE_COLOR_RED;
				cc_redblacktree_rotateRight(tree, cursor_x_node->parent_node);
				cursor_w_node = cursor_x_node->parent_node->left_node;
			}
			if (cursor_w_node->right_node->color == CC_REDBLACKTREE_COLOR_BLACK
					&& cursor_w_node->left_node->color
							== CC_REDBLACKTREE_COLOR_BLACK)
			{
				cursor_w_node->color = CC_REDBLACKTREE_COLOR_RED;
				cursor_x_node = cursor_x_node->parent_node;
			}
			else
			{
				if (cursor_w_node->left_node->color
						== CC_REDBLACKTREE_COLOR_BLACK)
				{
					cursor_w_node->right_node->color
							= CC_REDBLACKTREE_COLOR_BLACK;
					cursor_w_node->color = CC_REDBLACKTREE_COLOR_RED;
					cc_redblacktree_rotateLeft(tree, cursor_w_node);
					cursor_w_node = cursor_x_node->parent_node->left_node;
				}
				cursor_w_node->color = cursor_x_node->parent_node->color;
				cursor_x_node->parent_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cursor_w_node->left_node->color = CC_REDBLACKTREE_COLOR_BLACK;
				cc_redblacktree_rotateRight(tree, cursor_x_node->parent_node);
				cursor_x_node = properties->root_node;
			}
		}
	}
	cursor_x_node->color = CC_REDBLACKTREE_COLOR_BLACK;
	return;
}

cc_redblacktree_node *cc_redblacktree_findNodeWhenEqual(cc_redblacktree *tree,
		cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_node->key);
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

cc_redblacktree_node *cc_redblacktree_findNodeWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_node->key);
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

cc_redblacktree_node *cc_redblacktree_findNodeWhenGreater(
		cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_node->key);
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

cc_redblacktree_node *cc_redblacktree_findNodeWhenGreatest(
		cc_redblacktree *tree)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_node *target_node;

	do
	{
		if (tree == NULL)
		{
			target_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		target_node = &g_cc_redblacktree_node_nil;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			target_node = cursor_node;
			cursor_node = cursor_node->right_node;
		}
	} while (0);
	return target_node;
}

cc_redblacktree_node *cc_redblacktree_findNodeWhenLessEqual(
		cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_node->key);
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

cc_redblacktree_node *cc_redblacktree_findNodeWhenLess(cc_redblacktree *tree,
		cc_object *key)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	int find_way;

	do
	{
		if (tree == NULL || key == NULL)
		{
			cursor_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		while (cursor_node != &g_cc_redblacktree_node_nil)
		{
			find_way = key->compare(key, cursor_node->key);
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

cc_redblacktree_node *cc_redblacktree_findNodeWhenLeast(cc_redblacktree *tree)
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_node *target_node;

	do
	{
		if (tree == NULL)
		{
			target_node = NULL;
			break;
		}
		properties = tree->properties;
		cursor_node = properties->root_node;
		target_node = &g_cc_redblacktree_node_nil;
		while (cursor_node != &g_cc_redblacktree_node_nil)
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
void cc_redblacktree_traverse_preorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		callback(callbackobject, node->key, node->object);
		cc_redblacktree_traverse_preorder(node->left_node, callbackobject,
				callback);
		cc_redblacktree_traverse_preorder(node->right_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traversePreorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		properties = tree->properties;
		cc_redblacktree_traverse_preorder(properties->root_node,
				callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traversePreorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cc_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traversePreorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreater(tree, key);
		cc_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traversePreorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLessEqual(tree, key);
		cc_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traversePreorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLess(tree, key);
		cc_redblacktree_traverse_preorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/*
 * Calls a callback to the node.
 */
void cc_redblacktree_traverse_inorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		cc_redblacktree_traverse_inorder(node->left_node, callbackobject,
				callback);
		callback(callbackobject, node->key, node->object);
		cc_redblacktree_traverse_inorder(node->right_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traverseInorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		properties = tree->properties;
		cc_redblacktree_traverse_inorder(properties->root_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traverseIneorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cc_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traverseIneorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreater(tree, key);
		cc_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traverseIneorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLessEqual(tree, key);
		cc_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traverseIneorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLess(tree, key);
		cc_redblacktree_traverse_inorder(cursor_node, callbackobject, callback);
	}
	return result;
}

/*
 * Calls a callback to the node.
 */
void cc_redblacktree_traverse_postorder(cc_redblacktree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_redblacktree_node_nil)
	{
		cc_redblacktree_traverse_postorder(node->left_node, callbackobject,
				callback);
		cc_redblacktree_traverse_postorder(node->right_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traversePostorder(cc_redblacktree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_redblacktree_properties *properties;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		properties = tree->properties;
		cc_redblacktree_traverse_postorder(properties->root_node,
				callbackobject, callback);
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
cc_redblacktree_status cc_redblacktree_traversePostorderWhenGreaterEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreaterEqual(tree, key);
		cc_redblacktree_traverse_postorder(cursor_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traversePostorderWhenGreater(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenGreater(tree, key);
		cc_redblacktree_traverse_postorder(cursor_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traversePostorderWhenLessEqual(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLessEqual(tree, key);
		cc_redblacktree_traverse_postorder(cursor_node, callbackobject,
				callback);
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
cc_redblacktree_status cc_redblacktree_traversePostorderWhenLess(
		cc_redblacktree *tree, cc_object *key, cc_object *callbackobject,
		void(*callback)(cc_object *callbackobject, cc_object *key,
				cc_object *object))
{
	cc_redblacktree_node *cursor_node;
	cc_redblacktree_status result = CC_REDBLACKTREE_STATUS_SUCCESS;
	if (tree == NULL || key == NULL || callback == NULL)
	{
		result = CC_REDBLACKTREE_STATUS_ERROR;
	}
	else
	{
		cursor_node = cc_redblacktree_findNodeWhenLess(tree, key);
		cc_redblacktree_traverse_postorder(cursor_node, callbackobject,
				callback);
	}
	return result;
}

int cc_redblacktree_contain(cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	int result = 0;
	if (tree != NULL && key != NULL)
	{
		cursor_node = cc_redblacktree_findNodeWhenEqual(tree, key);
		if (cursor_node != &g_cc_redblacktree_node_nil)
		{
			result = 1;
		}
	}
	return result;
}

cc_object *cc_redblacktree_get(cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenEqual(tree, key);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenGreaterEqual(cc_redblacktree *tree,
		cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenGreaterEqual(tree, key);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenGreater(cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenGreater(tree, key);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenGreatest(cc_redblacktree *tree)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenGreatest(tree);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenLessEqual(cc_redblacktree *tree,
		cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenLessEqual(tree, key);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenLess(cc_redblacktree *tree, cc_object *key)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenLess(tree, key);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_redblacktree_getWhenLeast(cc_redblacktree *tree)
{
	cc_redblacktree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_redblacktree_findNodeWhenLeast(tree);
	if (cursor_node != &g_cc_redblacktree_node_nil)
	{
		result_object = cc_redblacktree_node_getObject(cursor_node);
	}
	return result_object;
}

void cc_redblacktree_tocstring_sub(cc_object *callbackobject, cc_object *key, cc_object *object)
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
	cc_object_release(key_str);
	cc_object_release(obj_str);
	free(key_cstr);
	free(obj_cstr);

	cc_object_release(str1);
	cc_object_release(str2);
	cc_object_release(str3);

	return;
}

char *cc_redblacktree_tocstring(cc_redblacktree *table)
{
	cc_string *str;
	char *cstring = NULL;

	str = cc_string_new("");
	cc_redblacktree_traversePreorder(table, str, cc_redblacktree_tocstring_sub);
	cstring = str->tocstring(str);
	cc_object_release(str);
	return cstring;
}

/*
 * For debug, prints a tree to the stdout.
 */
void cc_redblacktree_printNode(cc_redblacktree *tree,
		cc_redblacktree_node *node, int depth)
{
	int i;
	char *cs_node_key;

	if (node != &g_cc_redblacktree_node_nil)
	{
		cc_redblacktree_printNode(tree, node->left_node, depth + 1);
		for (i = 0; i < depth; i++)
		{
			printf("*");
		}
		cs_node_key = node->key->tocstring(node->key);
		printf("%s\n", cs_node_key);
		free(cs_node_key);
		cc_redblacktree_printNode(tree, node->right_node, depth + 1);
	}

}

