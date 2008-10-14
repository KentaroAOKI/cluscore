/*
 * ClusCore
 * cc_hashtable
 */

#include "cc_hashtable.h"
#include <stdlib.h>
#include <string.h>

/* private */
cc_hashtable_properties *cc_hashtable_properties_new(int size);
void cc_hashtable_properties_dispose(cc_hashtable_properties *properties);
cc_hashtable_node *cc_hashtable_node_new(char *key, cc_object *object);
void cc_hashtable_node_dispose(cc_hashtable_node *node);
unsigned int cc_hashtable_calchashvalue(cc_hashtable *table, char *key);

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
				cc_arraylist_dispose(properties->table[index]);
			}
			free(properties->table);
		}
		/* disposes cc_hashtable_properties */
		free(properties);
	}
	return;
}

cc_hashtable_node *cc_hashtable_node_new(char *key, cc_object *object)
{
	cc_hashtable_node *node;
	node = malloc(sizeof(cc_hashtable_node));
	if (node != NULL) {
		node->key = strdup(key);
		cc_object_grab(object);
		node->object = object;
	}
	return node;
}

void cc_hashtable_node_dispose(cc_hashtable_node *node)
{
	if (node != NULL) {
		free(node->key);
		cc_object_release(node->object);
		free(node);
	}
}

cc_hashtable *cc_hashtable_new(int size)
{
	cc_hashtable *object = NULL;
	cc_hashtable_properties *properties = cc_hashtable_properties_new(size);
	object = cc_object_new(&g_cc_hashtable_object_id, properties, cc_hashtable_properties_dispose);
	object->tocstring = (void *)cc_hashtable_tocstring;
	return (object);
}

void cc_hashtable_dispose(cc_hashtable *hash)
{
	cc_object_release(hash);
	return;
}

unsigned int cc_hashtable_calchashvalue(cc_hashtable *table, char *key)
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
	cc_object *leaf_object;
	cc_hashtable_node *leaf_node;
	unsigned int hashvalue;

	if (table != NULL && key != NULL)
	{
		properties = table->properties;
		hashvalue = cc_hashtable_calchashvalue(table, key);
		node = properties->table[hashvalue];
		for (cc_arraylist_setCursor(node, 0)
				;(leaf_object = cc_arraylist_getAtCursor(node)) != NULL
				;cc_arraylist_nextCursor(node))
		{
			leaf_node = (cc_hashtable_node *)leaf_object->properties;
			if (strcmp(key, leaf_node->key) == 0)
			{
				cc_arraylist_removeAtCursor(node);
				break;
			}
		}
		
	}
	return;
}

cc_object *cc_hashtable_get(cc_hashtable *table, char *key)
{
	cc_object *result_object = NULL;
	return result_object;
}

int cc_hashtable_exist(cc_hashtable *table, char *key)
{
	int result;
	return result;
}

char *cc_hashtable_tocstring(cc_hashtable *object)
{
	char *cstring;
	cstring = strdup("");
	return(cstring);
}

