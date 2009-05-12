/*
 * This code is a object for the ClusCore that it is the binary tree.
 * It has the "cc_binarytree_" prefix.
 * http://www.cluscore.com/
 */

#include "cc_binarytree.h"
#include "cc_string.h"
#include "cc_container.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* printf */

/*
 * Defines prototypes and Structures that it is private.
 */
typedef struct cc_binarytree_properties cc_binarytree_properties;
typedef struct cc_binarytree_node cc_binarytree_node;

/** Defines a struct of a node for the cc_binarytree. */
struct cc_binarytree_node
{
	cc_object *key;
	cc_object *object;
	cc_binarytree_node *parent_node;
	cc_binarytree_node *left_node;
	cc_binarytree_node *right_node;
};

/** Defines a properties of a object for the cc_binarytree. */
struct cc_binarytree_properties
{
	cc_binarytree_node *root_node;
	cc_binarytree_node *cursor_node;
	int count;
};

/* Defines prototypes of the cc_binarytree for private. */
cc_binarytree_properties *cc_binarytree_properties_new();
void cc_binarytree_properties_dispose(cc_binarytree_properties *properties);
cc_binarytree_node *cc_binarytree_node_new(cc_object *key, cc_object *object);
void cc_binarytree_node_dispose(cc_binarytree_node *node);
void cc_binarytree_node_swap(cc_binarytree_node *node1,
		cc_binarytree_node *node2);
void cc_binarytree_node_traversePreorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node));
void cc_binarytree_node_traverseInorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node));
void cc_binarytree_node_traversePostorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node));
void cc_binarytree_printNode(cc_binarytree *tree, cc_binarytree_node *node,
		int depth);

/** Defines the Nil-Node for cc_binarytree. */
cc_binarytree_node g_cc_binarytree_node_nil =
{ NULL, NULL, NULL, NULL, NULL };

/** Defines the Object-ID of binarytree. */
int g_cc_binarytree_object_id;

/**
 * Allocates a properties of the cc_binarytree when dealt by cc_binarytree_new().
 * @return a properties of cc_binarytree.
 */
cc_binarytree_properties *cc_binarytree_properties_new()
{
	cc_binarytree_properties *properties = NULL;
	do
	{
		/* creates cc_binarytree_properties */
		properties = malloc(sizeof(cc_binarytree_properties));
		if (properties == NULL)
		{
			break;
		}
		/* sets values */
		properties->root_node = &g_cc_binarytree_node_nil;
		properties->count = 0;
	} while (0);
	return properties;
}

/**
 * Frees a properties of the cc_binarytree when dealt by cc_binarytree_release().
 * @properties a properties of cc_binarytree to free.
 */
void cc_binarytree_properties_dispose(cc_binarytree_properties *properties)
{
	if (properties != NULL)
	{
		/* disposes cc_binarytree_properties */
		cc_binarytree_node_traversePostorder(properties->root_node,
				&cc_binarytree_node_dispose);
		free(properties);
	}
	return;
}

/**
 * Creates a object of the cc_binarytree.
 * @return a object of the cc_binarytree.
 */
cc_binarytree *cc_binarytree_new()
{
	cc_binarytree *object = NULL;
	cc_binarytree_properties *properties = cc_binarytree_properties_new();
	object = cc_object_new(&g_cc_binarytree_object_id, properties,
			cc_binarytree_properties_dispose);
	/* sets callback functions. */
	object->tocstring = (void *) cc_binarytree_tocstring;
	return (object);
}

/*
 * Releases a object of the cc_binarytree.
 * @tree a object of cc_binarytree to free.
 */
void cc_binarytree_release(cc_binarytree *tree)
{
	cc_object_release(tree);
	return;
}

/*
 * Allocates a node of tree.
 */
cc_binarytree_node *cc_binarytree_node_new(cc_object *key, cc_object *object)
{
	cc_binarytree_node *node = NULL;
	do
	{
		if (key == NULL)
		{
			break;
		}
		node = malloc(sizeof(cc_binarytree_node));
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
		node->parent_node = &g_cc_binarytree_node_nil;
		node->left_node = &g_cc_binarytree_node_nil;
		node->right_node = &g_cc_binarytree_node_nil;
	} while (0);
	return node;
}

/*
 * Frees a node of tree.
 */
void cc_binarytree_node_dispose(cc_binarytree_node *node)
{
	if (node != NULL)
	{
		cc_object_release(node->key);
		cc_object_release(node->object);
		free(node);
	}
	return;
}

/*
 * Gets a key object from a node.
 */
cc_object *cc_binarytree_node_getKey(cc_binarytree_node *node)
{
	cc_object *key = NULL;

	if (node != NULL)
	{
		key = node->key;
		if (key != NULL)
		{
			cc_object_grab(key);
		}
	}
	return key;
}

/*
 * Gets a object from a node.
 */
cc_object *cc_binarytree_node_getObject(cc_binarytree_node *node)
{
	cc_object *object = NULL;

	if (node != NULL)
	{
		object = node->object;
		if (object != NULL)
		{
			cc_object_grab(object);
		}
	}
	return object;
}

/*
 * Gets a c-stype string of a node.
 */
char *cc_binarytree_node_tocstring(cc_binarytree_node *node)
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
void cc_binarytree_node_swap(cc_binarytree_node *node1,
		cc_binarytree_node *node2)
{
	cc_object *tmp_key;
	cc_object *tmp_object;

	if (node1 != NULL && node2 != NULL)
	{
		tmp_key = node1->key;
		tmp_object = node1->object;
		node1->key = node2->key;
		node1->object = node2->object;
		node2->key = tmp_key;
		node2->object = tmp_object;
	}
	return;
}

/*
 * Gets a reference of maximum-node.
 */
cc_binarytree_node *cc_binarytree_node_maximum(cc_binarytree_node *node)
{
	while (node->left_node != &g_cc_binarytree_node_nil)
	{
		node = node->left_node;
	}
	return node;
}

/*
 * Gets a reference of minimum-node.
 */
cc_binarytree_node *cc_binarytree_node_minimum(cc_binarytree_node *node)
{
	while (node->right_node != &g_cc_binarytree_node_nil)
	{
		node = node->right_node;
	}
	return node;
}

/*
 * Gets a reference of successor-node.
 */
cc_binarytree_node *cc_binarytree_node_successor(cc_binarytree_node *node)
{
	cc_binarytree_node *cursor_node;
	if (node->right_node != &g_cc_binarytree_node_nil)
	{
		return cc_binarytree_node_minimum(node);
	}
	cursor_node = node->parent_node;
	while (cursor_node != &g_cc_binarytree_node_nil && node
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
void cc_binarytree_node_traversePreorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		cb(node);
		cc_binarytree_node_traversePreorder(node->left_node, cb);
		cc_binarytree_node_traversePreorder(node->right_node, cb);
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
void cc_binarytree_node_traverseInorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		cc_binarytree_node_traverseInorder(node->left_node, cb);
		cb(node);
		cc_binarytree_node_traverseInorder(node->right_node, cb);
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
void cc_binarytree_node_traversePostorder(cc_binarytree_node *node, void(*cb)(
		cc_binarytree_node *node))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		cc_binarytree_node_traversePostorder(node->left_node, cb);
		cc_binarytree_node_traversePostorder(node->right_node, cb);
		cb(node);
	}
	return;
}

/**
 * Rotates a tree in left.
 */
void cc_binarytree_rotateLeft(cc_binarytree *tree, cc_binarytree_node *node)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;

	properties = tree->properties;
	cursor_node = node->right_node;
	node->right_node = cursor_node->left_node;
	if (cursor_node->left_node != &g_cc_binarytree_node_nil)
	{
		cursor_node->left_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cc_binarytree_node_nil)
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
void cc_binarytree_rotateRight(cc_binarytree *tree, cc_binarytree_node *node)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;

	properties = tree->properties;
	cursor_node = node->left_node;
	node->left_node = cursor_node->right_node;
	if (cursor_node->right_node != &g_cc_binarytree_node_nil)
	{
		cursor_node->right_node->parent_node = node;
	}
	cursor_node->parent_node = node->parent_node;
	if (node->parent_node == &g_cc_binarytree_node_nil)
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
 * Inserts a node to a tree.
 */
void cc_binarytree_insert(cc_binarytree *tree, cc_object *key,
		cc_object *object)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *insert_node;
	cc_binarytree_node *cursor_node;
	cc_binarytree_node *leaf_node;
	int insert_way = 0;

	properties = tree->properties;
	/* creates a node to insert an object. */
	insert_node = cc_binarytree_node_new(key, object);
	/* moves a cursor to leaf. */
	leaf_node = &g_cc_binarytree_node_nil;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	if (cursor_node == &g_cc_binarytree_node_nil)
	{
		/* connects a node to this tree. */
		insert_node->parent_node = leaf_node;
		if (leaf_node == &g_cc_binarytree_node_nil)
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
	}
	else
	{
		/* replaces a node to this tree. */
		cc_binarytree_node_swap(cursor_node, insert_node);
		cc_binarytree_node_dispose(insert_node);
	}
	return;
}

/**
 * Deletes a node from a tree.
 */
void cc_binarytree_delete(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *delete_node;
	cc_binarytree_node *cursor_x_node;
	cc_binarytree_node *cursor_y_node;
	int find_way;

	properties = tree->properties;
	/* finds a node to delete it from a tree. */
	delete_node = &g_cc_binarytree_node_nil;
	cursor_y_node = properties->root_node;
	while (cursor_y_node != &g_cc_binarytree_node_nil)
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
	if (delete_node == &g_cc_binarytree_node_nil)
	{
		return;
	}
	if (delete_node->left_node == &g_cc_binarytree_node_nil
			|| delete_node->right_node == &g_cc_binarytree_node_nil)
	{
		cursor_y_node = delete_node;
	}
	else
	{
		cursor_y_node = cc_binarytree_node_successor(delete_node);
	}
	if (cursor_y_node->left_node != &g_cc_binarytree_node_nil)
	{
		cursor_x_node = cursor_y_node->left_node;
	}
	else
	{
		cursor_x_node = cursor_y_node->right_node;
	}
	cursor_x_node->parent_node = cursor_y_node->parent_node;
	if (cursor_y_node->parent_node == &g_cc_binarytree_node_nil)
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
		cc_binarytree_node_swap(delete_node, cursor_y_node);

	}
	cc_binarytree_node_dispose(cursor_y_node);
	return;
}

cc_binarytree_node *cc_binarytree_findNodeWhenEqual(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	int find_way;

	properties = tree->properties;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	return cursor_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	int find_way;

	properties = tree->properties;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	return cursor_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenGreater(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	int find_way;

	properties = tree->properties;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	return cursor_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenGreatest(cc_binarytree *tree)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	cc_binarytree_node *target_node;

	properties = tree->properties;
	cursor_node = properties->root_node;
	target_node = &g_cc_binarytree_node_nil;
	while (cursor_node != &g_cc_binarytree_node_nil)
	{
		target_node = cursor_node;
		cursor_node = cursor_node->right_node;
	}
	return target_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenLessEqual(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	int find_way;

	properties = tree->properties;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	return cursor_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenLess(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	int find_way;

	properties = tree->properties;
	cursor_node = properties->root_node;
	while (cursor_node != &g_cc_binarytree_node_nil)
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
	return cursor_node;
}

cc_binarytree_node *cc_binarytree_findNodeWhenLeast(cc_binarytree *tree)
{
	cc_binarytree_properties *properties;
	cc_binarytree_node *cursor_node;
	cc_binarytree_node *target_node;

	properties = tree->properties;
	cursor_node = properties->root_node;
	target_node = &g_cc_binarytree_node_nil;
	while (cursor_node != &g_cc_binarytree_node_nil)
	{
		target_node = cursor_node;
		cursor_node = cursor_node->left_node;
	}
	return target_node;
}

/*
 * Calls a callback to the node.
 */
void cc_binarytree_reprocessTraversePreorder(cc_binarytree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		callback(callbackobject, node->key, node->object);
		cc_binarytree_reprocessTraversePreorder(node->left_node,
				callbackobject, callback);
		cc_binarytree_reprocessTraversePreorder(node->right_node,
				callbackobject, callback);
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
void cc_binarytree_traversePreorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_binarytree_properties *properties;
	properties = tree->properties;
	cc_binarytree_reprocessTraversePreorder(properties->root_node,
			callbackobject, callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePreorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreaterEqual(tree, key);
	cc_binarytree_reprocessTraversePreorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePreorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreater(tree, key);
	cc_binarytree_reprocessTraversePreorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePreorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLessEqual(tree, key);
	cc_binarytree_reprocessTraversePreorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in preorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePreorderWhenLess(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLess(tree, key);
	cc_binarytree_reprocessTraversePreorder(cursor_node, callbackobject,
			callback);
	return;
}

/*
 * Calls a callback to the node.
 */
void cc_binarytree_reprocessTraverseInorder(cc_binarytree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		cc_binarytree_reprocessTraverseInorder(node->left_node, callbackobject,
				callback);
		callback(callbackobject, node->key, node->object);
		cc_binarytree_reprocessTraverseInorder(node->right_node,
				callbackobject, callback);
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
void cc_binarytree_traverseInorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_binarytree_properties *properties;
	properties = tree->properties;
	cc_binarytree_reprocessTraverseInorder(properties->root_node,
			callbackobject, callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traverseIneorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreaterEqual(tree, key);
	cc_binarytree_reprocessTraverseInorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traverseIneorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreater(tree, key);
	cc_binarytree_reprocessTraverseInorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traverseIneorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLessEqual(tree, key);
	cc_binarytree_reprocessTraverseInorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in inorder,
 * perform the following operations recursively at each node,
 * starting with the node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traverseIneorderWhenLess(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLess(tree, key);
	cc_binarytree_reprocessTraverseInorder(cursor_node, callbackobject,
			callback);
	return;
}

/*
 * Calls a callback to the node.
 */
void cc_binarytree_reprocessTraversePostorder(cc_binarytree_node *node,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	if (node != &g_cc_binarytree_node_nil)
	{
		cc_binarytree_reprocessTraversePostorder(node->left_node,
				callbackobject, callback);
		cc_binarytree_reprocessTraversePostorder(node->right_node,
				callbackobject, callback);
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
void cc_binarytree_traversePostorder(cc_binarytree *tree,
		cc_object *callbackobject, void(*callback)(cc_object *callbackobject,
				cc_object *key, cc_object *object))
{
	cc_binarytree_properties *properties;
	properties = tree->properties;
	cc_binarytree_reprocessTraversePostorder(properties->root_node,
			callbackobject, callback);
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePostorderWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreaterEqual(tree, key);
	cc_binarytree_reprocessTraversePostorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when greater than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePostorderWhenGreater(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenGreater(tree, key);
	cc_binarytree_reprocessTraversePostorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than or equal to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePostorderWhenLessEqual(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLessEqual(tree, key);
	cc_binarytree_reprocessTraversePostorder(cursor_node, callbackobject,
			callback);
	return;
}

/**
 * Traverses a tree in postorder,
 * perform the following operations recursively at each node,
 * starting with a node when less than to a key.
 * 1. Traverse the left subtree.
 * 2. Visit the node.
 * 3. Traverse the right subtree.
 */
void cc_binarytree_traversePostorderWhenLess(cc_binarytree *tree,
		cc_object *key, cc_object *callbackobject, void(*callback)(
				cc_object *callbackobject, cc_object *key, cc_object *object))
{
	cc_binarytree_node *cursor_node;
	cursor_node = cc_binarytree_findNodeWhenLess(tree, key);
	cc_binarytree_reprocessTraversePostorder(cursor_node, callbackobject,
			callback);
	return;
}

int cc_binarytree_contain(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_node *cursor_node;
	int result = 0;
	cursor_node = cc_binarytree_findNodeWhenEqual(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result = 1;
	}
	return result;
}

cc_object *cc_binarytree_get(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenEqual(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenGreaterEqual(cc_binarytree *tree,
		cc_object *key)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenGreaterEqual(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenGreater(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenGreater(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenGreatest(cc_binarytree *tree)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenGreatest(tree);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenLessEqual(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenLessEqual(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenLess(cc_binarytree *tree, cc_object *key)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenLess(tree, key);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

cc_object *cc_binarytree_getWhenLeast(cc_binarytree *tree)
{
	cc_binarytree_node *cursor_node;
	cc_object *result_object = NULL;
	cursor_node = cc_binarytree_findNodeWhenLeast(tree);
	if (cursor_node != &g_cc_binarytree_node_nil)
	{
		result_object = cc_binarytree_node_getObject(cursor_node);
	}
	return result_object;
}

char *cc_binarytree_tocstring(cc_binarytree *table)
{
	char *cstring = NULL;
	return cstring;
}

/*
 * For debug, prints a tree to the stdout.
 */
void cc_binarytree_printNode(cc_binarytree *tree, cc_binarytree_node *node,
		int depth)
{
	int i;
	char *cs_node_key;

	if (node != &g_cc_binarytree_node_nil)
	{
		cc_binarytree_printNode(tree, node->left_node, depth + 1);
		for (i = 0; i < depth; i++)
		{
			printf("*");
		}
		cs_node_key = node->key->tocstring(node->key);
		printf("%s\n", cs_node_key);
		free(cs_node_key);
		cc_binarytree_printNode(tree, node->right_node, depth + 1);
	}

}

