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

cc_arraylist *cc_arraylist_new(void)
{
	cc_arraylist *object = NULL;
	cc_arraylist_properties *properties = cc_arraylist_properties_new();
	object = cc_object_new(&g_cc_arraylist_object_id, properties, cc_arraylist_properties_dispose);
	object->tocstring = (void *)cc_arraylist_tocstring;
	return (object);
}

void cc_arraylist_dispose(cc_arraylist *object)
{
	cc_object_release(object);
	return;
}

void cc_arraylist_add(cc_arraylist *object, cc_object *addobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;

	if (object != NULL && object->properties != NULL && addobject != NULL)
	{
		properties = object->properties;

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
		} else {
			properties->bottom->next = node;
			properties->bottom = node;
		}
	}
	return;
}

void cc_arraylist_set_cursor(cc_arraylist *object, int index)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	cc_arraylist_node *node_next;
	int count;

	if (object != NULL && object->properties != NULL)
	{
		properties = object->properties;
		if (properties->top != NULL && properties->length > index)
		{
			count = 0;
			for (node = properties->top; node != NULL; node = node_next)
			{
				node_next = node->next;
				if (index == count)
				{
					properties->cursor = node;
					break;
				}
			}
		}
	}
	return;
}

cc_object *cc_arraylist_get(cc_arraylist *object)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (object != NULL && object->properties != NULL)
	{
		properties = object->properties;
		if (properties->cursor != NULL)
		{
			result_object = properties->cursor->object;
			properties->cursor = properties->cursor->next;
			cc_object_grab(result_object);
		}
	}
	return result_object;
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

