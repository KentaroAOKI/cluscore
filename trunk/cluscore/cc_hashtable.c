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
 * The Hash Table Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#include "cc_hashtable.h"
#include "cc_string.h"
#include "cc_redblacktree.h"
#include <stdlib.h>
#include <string.h>

typedef struct cc_hashtable_properties cc_hashtable_properties;
typedef struct cc_hashtable_node cc_hashtable_node;

struct cc_hashtable_node
{
	cc_object *key;
	cc_object *object;
};

struct cc_hashtable_properties
{
	int size;
	int count;
	int collision;
	cc_redblacktree **table;
};

cc_hashtable_properties *cc_hashtable_properties_new(int size);
void cc_hashtable_properties_dispose(cc_hashtable_properties *properties);
cc_hashtable_node *cc_hashtable_node_new(cc_object *key, cc_object *object);
void cc_hashtable_node_dispose(cc_hashtable_node *node);
unsigned int cc_hashtable_getHash(cc_hashtable *table, cc_object *key);

int g_cc_hashtable_object_id;

cc_hashtable_properties *cc_hashtable_properties_new(int size)
{
	cc_hashtable_properties *properties = NULL;
	cc_hashtable_properties *result_properties = NULL;
	int index;
	do
	{
		/* creates cc_hashtable_properties */
		properties = malloc(sizeof(cc_hashtable_properties));
		if (properties == NULL)
		{
			break;
		}
		memset(properties, 0, sizeof(cc_hashtable_properties));
		/* creates tables */
		properties->table = malloc(sizeof(cc_hashtable_node *) * size);
		if (properties->table == NULL)
		{
			break;
		}
		for (index = 0; index < size; index++)
		{
			properties->table[index] = cc_redblacktree_new();
		}
		/* sets value */
		properties->size = size;
		properties->collision = 0;
		result_properties = properties;
	} while (0);
	/* checks result */
	if (result_properties == NULL)
	{
		cc_hashtable_properties_dispose(properties);
	}
	return result_properties;
}

void cc_hashtable_properties_dispose(cc_hashtable_properties *properties)
{
	int index;
	if (properties != NULL)
	{
		/* disposes tables */
		if (properties->table != NULL)
		{
			for (index = 0; index < properties->size; index++)
			{
				cc_redblacktree_release(properties->table[index]);
			}
			free(properties->table);
		}
		/* disposes cc_hashtable_properties */
		free(properties);
	}
	return;
}

cc_hashtable *cc_hashtable_new(int size)
{
	cc_hashtable *object = NULL;
	cc_hashtable_properties *properties = cc_hashtable_properties_new(size);
	object = cc_object_new(&g_cc_hashtable_object_id, properties,
			cc_hashtable_properties_dispose);
	object->tocstring = (void *) cc_hashtable_tocstring;
	return (object);
}

void cc_hashtable_release(cc_hashtable *hash)
{
	cc_object_release(hash);
	return;
}

cc_hashtable_node *cc_hashtable_node_new(cc_object *key, cc_object *object)
{
	cc_hashtable_node *node;
	node = malloc(sizeof(cc_hashtable_node));
	if (node != NULL)
	{
		if (key != NULL)
		{
			cc_object_grab(key);
		}
		node->key = key;
		if (object != NULL)
		{
			cc_object_grab(object);
		}
		node->object = object;
	}
	return node;
}

void cc_hashtable_node_dispose(cc_hashtable_node *node)
{
	if (node != NULL)
	{
		if (node->key != NULL)
		{
			cc_object_release(node->key);
		}
		if (node->object != NULL)
		{
			cc_object_release(node->object);
		}
		free(node);
	}
}

cc_object *cc_hashtable_node_getObject(cc_hashtable_node *node)
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

char *cc_hashtable_node_tocstring(cc_hashtable_node *node)
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
		cstring = node->object->tocstring(node->key);
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

int cc_hashtable_node_compare(cc_hashtable_node *node1,
		cc_hashtable_node *node2)
{
	int result = -1;
	if (node1 != NULL && node2 != NULL)
	{
		result = node1->key->compare(node1->key, node2->key);
	}
	return result;
}

unsigned int cc_hashtable_getHash1(cc_hashtable *table, cc_object *key)
{
	cc_hashtable_properties *properties;
	unsigned int hashvalue = 0;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = key->get_hashcode(key, 121212);
		hashvalue = hashvalue % properties->size;
	}
	return hashvalue;
}

unsigned int cc_hashtable_getHash2(cc_hashtable *table, cc_object *key)
{
	cc_hashtable_properties *properties;
	unsigned int hashvalue = 0;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = key->get_hashcode(key, 343434);
		hashvalue = hashvalue % (properties->size);
	}
	return hashvalue;
}

void cc_hashtable_add(cc_hashtable *table, cc_object *key, cc_object *object)
{
	cc_hashtable_properties *properties;
	cc_redblacktree *node;
	unsigned int hashvalue;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = cc_hashtable_getHash1(table, key);
		node = properties->table[hashvalue];
		cc_redblacktree_insert(node, key, object);
	}
	return;
}

void cc_hashtable_set(cc_hashtable *table, cc_object *key, cc_object *object)
{
	cc_hashtable_properties *properties;
	cc_redblacktree *node;
	unsigned int hashvalue;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = cc_hashtable_getHash1(table, key);
		node = properties->table[hashvalue];
		cc_redblacktree_insertWithReplace(node, key, object);
	}
	return;
}

cc_object *cc_hashtable_get(cc_hashtable *table, cc_object *key)
{
	cc_object *result = NULL;
	cc_hashtable_properties *properties;
	cc_redblacktree *node;
	unsigned int hashvalue;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = cc_hashtable_getHash1(table, key);
		node = properties->table[hashvalue];
		result = cc_redblacktree_get(node, key);
	}
	return result;
}

int cc_hashtable_contain(cc_hashtable *table, cc_object *key)
{
	cc_hashtable_properties *properties;
	cc_redblacktree *node;
	unsigned int hashvalue;
	int result = 0;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = cc_hashtable_getHash1(table, key);
		node = properties->table[hashvalue];
		result = cc_redblacktree_contain(node, key);
	}
	return result;
}

int cc_hashtable_collision(cc_hashtable *table)
{
	int result = -1;
	cc_hashtable_properties *properties;

	if (table != NULL)
	{
		properties = table->properties;
		result = properties->collision;
	}
	return result;
}

char *cc_hashtable_tocstring(cc_hashtable *table)
{
	cc_hashtable_properties *properties;
	cc_string *result_string;
	cc_string *string;
	int index;
	char *cstring;

	properties = table->properties;
	result_string = cc_string_new("");
	for (index = 0; index < properties->size; index++)
	{
		cstring = properties->table[index]->tocstring(properties->table[index]);
		string = cc_string_new(cstring);
		cc_string_catenate(result_string, string);
		cc_string_release(string);
		string = cc_string_new("\n");
		cc_string_catenate(result_string, string);
		cc_string_release(string);
		free(cstring);
	}
	cstring = result_string->tocstring(result_string);
	cc_string_release(result_string);
	return cstring;
}

