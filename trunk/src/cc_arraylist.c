/*
 * ClusCore
 * cc_arraylist
 */

#include "cc_arraylist.h"
#include <stdlib.h>
#include <string.h>

int g_cc_arraylist_object_id;

cc_arraylist_properties *cc_arraylist_properties_new(void)
{
	cc_arraylist_properties *properties;
	properties = malloc(sizeof(cc_arraylist_properties));
	if (properties != NULL)
	{
		memset(properties, 0, sizeof(cc_arraylist_properties));
	}
	return properties;
}

void cc_arraylist_properties_dispose(cc_arraylist_properties *properties)
{
	cc_arraylist_node *node;
	cc_arraylist_node *node_next;
	if (properties != NULL)
	{
		if (properties->top != NULL)
		{
			for (node = properties->top; node != NULL; node = node_next)
			{
				node_next = node->next;
				cc_object_release(node->object);
				free(node);
			}
		}
		free(properties);
	}
	return;
}

cc_arraylist_node *cc_arraylist_node_get(cc_arraylist *list, int index)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	int count;
	int diff_index;
	int countway;

	if (list != NULL && list->properties != NULL)
	{
		properties = list->properties;
		if (properties->top != NULL && properties->length > index)
		{
			diff_index = index - properties->cache_index;
//			if (properties->cache_node != NULL && index >= abs(diff_index))
			if (0)
			{
				countway = diff_index;
				count = properties->cache_index;
				node = properties->cache_node;
			} else {
				countway = 1;
				count = 0;
				node = properties->top;
			}
			if (countway > 0)
			{
				while (node != NULL)
				{
					if (count == index)
					{
						break;
					}
					count ++;
					node = node->next;
				}
			} else if (countway < 0) {
				while (node != NULL)
				{
					count --;
					node = node->next;
					if (count == index)
					{
						break;
					}
				}
			}
			properties->cache_index = count;
			properties->cache_node = node;
		}
	}
	return node;
}

cc_arraylist *cc_arraylist_new(void)
{
	cc_arraylist *object = NULL;
	cc_arraylist_properties *properties = cc_arraylist_properties_new();
	object = cc_object_new(&g_cc_arraylist_object_id, properties, cc_arraylist_properties_dispose);
	object->tocstring = (void *)cc_arraylist_tocstring;
	return (object);
}

void cc_arraylist_dispose(cc_arraylist *list)
{
	cc_object_release(list);
	return;
}

int cc_arraylist_getLength(cc_arraylist *list)
{
	int result = -1;
	cc_arraylist_properties *properties;
	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		result = properties->length;
	}
	return result;
}

int cc_arraylist_setCursor(cc_arraylist *list, int index)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		node = cc_arraylist_node_get(list, index);
		if (node != NULL) {
			properties->cursor = node;
			result = index;
		} else {
			properties->cursor = NULL;
		}
	}
	return result;
}

int cc_arraylist_prevCursor(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->cursor != NULL) {
			properties->cursor = properties->cursor->prev;
			if (properties->cursor != NULL) {
				result = 1;
			}
		}
	}
	return result;
}

int cc_arraylist_nextCursor(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->cursor != NULL) {
			properties->cursor = properties->cursor->next;
			if (properties->cursor != NULL) {
				result = 1;
			}
		}
	}
	return result;
}

void cc_arraylist_add(cc_arraylist *list, cc_object *addobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;

	if (list != NULL && list->properties != NULL && addobject != NULL)
	{
		properties = list->properties;

		cc_object_grab(addobject);

		/* Sets property in node. */
		node = malloc(sizeof(cc_arraylist_node));
		node->object = addobject;
		node->parent = properties;
		node->prev = properties->bottom;
		node->next = NULL;

		/* Adds node to an arraylist. */
		properties->length ++;
		if (properties->top == NULL)
		{
			/* first time. */
			properties->top = node;
			properties->bottom = node;
			properties->cursor = node;
		} else {
			properties->bottom->next = node;
			properties->bottom = node;
		}
		/* update cursor */
		properties->cursor = node;
	}
	return;
}

void cc_arraylist_insert(cc_arraylist *list, cc_object *insertobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;
	cc_arraylist_node *insertnode;

	if (list != NULL && list->properties != NULL && insertobject != NULL) {
		properties = list->properties;
		insertnode = properties->cursor;
		if (insertnode != NULL) {
			properties->length ++;
			cc_object_grab(insertobject);
			node = malloc(sizeof(cc_arraylist_node));
			node->object = insertobject;
			node->parent = properties;
			node->prev = insertnode->prev;
			node->next = insertnode;
			if (insertnode->prev != NULL) {
				insertnode->prev->next = node;
			} else {
				properties->top = node;
			}
			insertnode->prev = node;
			/* update cache */
			properties->cache_index ++;
			/* update cursor */
			properties->cursor = node;
		} else {
			cc_arraylist_add(list, insertobject);
		}
	}
	return;
}

void cc_arraylist_insertWithIndex(cc_arraylist *list, cc_object *insertobject, int index)
{
	cc_arraylist_setCursor(list, index);
	cc_arraylist_insert(list, insertobject);
	return;
}

void cc_arraylist_remove(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *removenode;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		removenode = properties->cursor;
		if (removenode != NULL) {
			properties->length --;
			cc_object_release(removenode->object);
			if (removenode->prev == NULL) {
				properties->top = removenode->next;
			} else {
				removenode->prev->next = removenode->next;
			}
			if (removenode->next == NULL) {
				properties->bottom = removenode->prev;
			} else {
				removenode->next->prev = removenode->prev;
			}
			/* update cache */
			properties->cache_node = properties->cache_node->next;
			if (properties->cache_node->next == NULL) {
				properties->cache_index = 0;
			}
			/* update cursor */
			properties->cursor = removenode->next;
			free(removenode);
		}
	}
	return;
}

void cc_arraylist_removeWithIndex(cc_arraylist *list, int index)
{
	cc_arraylist_setCursor(list, index);
	cc_arraylist_remove(list);
	return;
}

cc_object *cc_arraylist_get(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->cursor != NULL) {
			result_object = properties->cursor->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getWithIndex(cc_arraylist *list, int index)
{
	cc_arraylist_setCursor(list, index);
	return cc_arraylist_get(list);
}

char *cc_arraylist_tocstring(cc_arraylist *object)
{
	char *cstring = NULL;
	int cstring_length;
	int cstring_alloc_size;
	int add_alloc_size = 1024; /* It is bigger 1. */
	char *object_cstring = NULL;
	int object_cstring_length;
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	cc_arraylist_node *node_next;

	if (object != NULL && object->properties != NULL)
	{
		properties = object->properties;
		if (properties->top != NULL) {
			/* Initializes cstring buffer. */
			cstring = malloc(add_alloc_size);
			memset(cstring, 0, add_alloc_size);
			cstring_length = 0;
			cstring_alloc_size = add_alloc_size;
			/* Builds cstring. */
			for (node = properties->top; node != NULL; node = node_next)
			{
				node_next = node->next;
				if (node->object->tocstring == NULL)
				{
					continue;
				}
				object_cstring = node->object->tocstring(node->object);
				object_cstring_length = strlen(object_cstring);
				if (cstring_alloc_size <= (cstring_length + object_cstring_length))
				{
					cstring_alloc_size = cstring_length + object_cstring_length + add_alloc_size;
					cstring = realloc(cstring, cstring_alloc_size);
				}
				memcpy(cstring + cstring_length, object_cstring, object_cstring_length);
				cstring_length = cstring_length + object_cstring_length;
				cstring[cstring_length] = 0;
				free(object_cstring);
			}
		}
	}
	return(cstring);
}

