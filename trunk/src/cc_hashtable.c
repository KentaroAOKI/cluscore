/*
 * ClusCore
 * cc_hashtable
 */

#include "cc_hashtable.h"
#include "cc_string.h"
#include "cc_container.h"
#include <stdlib.h>
#include <string.h>

typedef struct cc_hashtable_properties cc_hashtable_properties;
typedef struct cc_hashtable_node cc_hashtable_node;

struct cc_hashtable_node {
	char *key;
	cc_object *object;
};

struct cc_hashtable_properties
{
	int size;
	cc_arraylist **table;
	int count;
};

cc_hashtable_properties *cc_hashtable_properties_new(int size);
void cc_hashtable_properties_dispose(cc_hashtable_properties *properties);
cc_hashtable_node *cc_hashtable_node_new(char *key, cc_object *object);
void cc_hashtable_node_dispose(cc_hashtable_node *node);
unsigned int cc_hashtable_getHash(cc_hashtable *table, char *key);

int g_cc_hashtable_object_id;

cc_hashtable_properties *cc_hashtable_properties_new(int size)
{
	cc_hashtable_properties *properties = NULL;
	cc_hashtable_properties *result_properties = NULL;
	int index;
	do {
		/* creates cc_hashtable_properties */
		properties = malloc(sizeof(cc_hashtable_properties));
		if (properties == NULL) {
			break;
		}
		memset (properties, 0, sizeof(cc_hashtable_properties));
		/* creates tables */
		properties->table = malloc(sizeof(cc_hashtable_node *) * size);
		if (properties->table == NULL) {
			break;
		}
		for (index = 0; index < size; index ++) {
			properties->table[index] = cc_arraylist_new();
		}
		/* sets value */
		properties->size = size;
		result_properties = properties;
	} while(0);
	/* checks result */
	if (result_properties == NULL) {
		cc_hashtable_properties_dispose(properties);
	}
	return result_properties;
}

void cc_hashtable_properties_dispose(cc_hashtable_properties *properties)
{
	int index;
	if (properties != NULL) {
		/* disposes tables */
		if (properties->table != NULL) {
			for (index = 0; index < properties->size; index ++) {
				cc_arraylist_release(properties->table[index]);
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
	object = cc_object_new(&g_cc_hashtable_object_id, properties, cc_hashtable_properties_dispose);
	object->tocstring = (void *)cc_hashtable_tocstring;
	return (object);
}

void cc_hashtable_release(cc_hashtable *hash)
{
	cc_object_release(hash);
	return;
}

cc_hashtable_node *cc_hashtable_node_new(char *key, cc_object *object)
{
	cc_hashtable_node *node;
	node = malloc(sizeof(cc_hashtable_node));
	if (node != NULL)
	{
		node->key = strdup(key);
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
		free(node->key);
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
	
	if (node != NULL) {
		string_base = cc_string_new("[");
		string_close = cc_string_new("]");
		string_key = cc_string_new(node->key);
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

int cc_hashtable_node_compare(cc_hashtable_node *node1, cc_hashtable_node *node2)
{
	int result = -1;
	if (node1 != NULL && node2 != NULL) {
		result = strcmp(node1->key, node2->key);
	}
	return result;
}

cc_container *cc_hashtable_node_container_new(char *key, cc_object *object)
{
	cc_container *container;
	cc_hashtable_node *node;
	node = cc_hashtable_node_new(key, object);
	container = cc_container_new(node, cc_hashtable_node_dispose);
	cc_container_setCompare(container, (void *)cc_hashtable_node_compare);
	cc_container_setTocstring(container, (void *)cc_hashtable_node_tocstring);
	return container;
}

void cc_hashtable_node_container_dispose(cc_container *container)
{
	cc_container_release(container);
	return;
}

cc_object *cc_hashtable_node_container_getObject(cc_container *container)
{
	cc_hashtable_node *node;
	cc_object *object;

	node = cc_container_getrefBuffer(container);
	object = cc_hashtable_node_getObject(node);
	return object;
}

unsigned int cc_hashtable_getHash(cc_hashtable *table, char *key)
{
	cc_hashtable_properties *properties;
	unsigned int hashvalue = 0;
	
	if (table != NULL && key != NULL) {
		properties = table->properties;
		hashvalue = 9010;
		while (*key != 0) {
			hashvalue = hashvalue * 37 + (unsigned char)*key;
			key ++;
		}
		hashvalue = hashvalue % properties->size;
	}
	return hashvalue;
}

void cc_hashtable_add(cc_hashtable *table, char *key, cc_object *object)
{
	return;
}

void cc_hashtable_set(cc_hashtable *table, char *key, cc_object *object)
{
	cc_hashtable_properties *properties;
	cc_arraylist *node;
	cc_container *leaf;
	int leafindex;
	unsigned int hashvalue;
	
	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		
		hashvalue = cc_hashtable_getHash(table, key);
		node = properties->table[hashvalue];

		leaf = cc_hashtable_node_container_new(key, object);
		leafindex = cc_arraylist_findForwardFromFront(node, leaf);
		if (leafindex < 0)
		{
			cc_arraylist_addAtFront(node, leaf);
		}
		else
		{
			cc_arraylist_removeAt(node, leafindex);
			cc_arraylist_addAtFront(node, leaf);
		}
		cc_hashtable_node_container_dispose(leaf);
	}
	return;
}

cc_object *cc_hashtable_get(cc_hashtable *table, char *key)
{
	cc_object *result_object = NULL;
	cc_hashtable_properties *properties;
	cc_arraylist *node;
	cc_container *leaf;
	cc_container *leaf_result;
	int leafindex;
	unsigned int hashvalue;
	
	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		
		hashvalue = cc_hashtable_getHash(table, key);
		node = properties->table[hashvalue];

		leaf = cc_hashtable_node_container_new(key, NULL);
		leafindex = cc_arraylist_findForwardFromFront(node, leaf);
		if (leafindex >= 0)
		{
			leaf_result = cc_arraylist_getAtCursor(node);
			result_object = cc_hashtable_node_container_getObject(leaf_result);
			cc_hashtable_node_container_dispose(leaf_result);
		}
		cc_hashtable_node_container_dispose(leaf);
	}
	return result_object;
}

int cc_hashtable_exist(cc_hashtable *table, char *key)
{
	int result;
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
	for (index = 0; index < properties->size; index ++) {
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

