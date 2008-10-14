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
		if (properties->front != NULL)
		{
			for (node = properties->front; node != NULL; node = node_next)
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

void cc_arraylist_dispose(cc_arraylist *list)
{
	cc_object_release(list);
	return;
}

int cc_arraylist_getLength(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	int result = -1;
	
	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		result = properties->length;
	}
	return result;
}

int cc_arraylist_setCursorAt(cc_arraylist *list, int index)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node = NULL;
	int count = -1;
	int diff_index;
	int countway;

	if (list != NULL && list->properties != NULL)
	{
		properties = list->properties;
		if (properties->front != NULL && properties->length > index && index >= 0)
		{
			diff_index = index - properties->cursor_index;
			if (properties->cursor != NULL && index >= abs(diff_index))
			{
				countway = diff_index;
				count = properties->cursor_index;
				node = properties->cursor;
			} else {
				countway = 1;
				count = 0;
				node = properties->front;
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
					node = node->previous;
					if (count == index)
					{
						break;
					}
				}
			}
			properties->cursor_index = count;
			properties->cursor = node;
		}
	}
	return count;
}

int cc_arraylist_previousCursor(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->cursor != NULL) {
			properties->cursor = properties->cursor->previous;
			properties->cursor_index --;
			result = properties->cursor_index;
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
			properties->cursor_index ++;
			if (properties->cursor != NULL) {
				result = properties->cursor_index;
			}
		}
	}
	return result;
}

int cc_arraylist_addAtFront(cc_arraylist *list, cc_object *addobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;
	int result = -1;

	if (list != NULL && list->properties != NULL && addobject != NULL)
	{
		properties = list->properties;

		cc_object_grab(addobject);

		/* Sets property in node. */
		node = malloc(sizeof(cc_arraylist_node));
		node->object = addobject;
		node->parent = properties;
		node->previous = NULL;
		node->next = properties->front;

		/* Adds node to an arraylist. */
		properties->length ++;
		if (properties->front == NULL)
		{
			/* first time. */
			properties->front = node;
			properties->back = node;
			properties->cursor = node;
		} else {
			properties->front->previous = node;
			properties->front = node;
		}
		/* update cursor */
		properties->cursor = node;
		properties->cursor_index = 0;
		result = properties->cursor_index;
	}
	return result;
}

int cc_arraylist_addAtBack(cc_arraylist *list, cc_object *addobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;
	int result = -1;

	if (list != NULL && list->properties != NULL && addobject != NULL)
	{
		properties = list->properties;

		cc_object_grab(addobject);

		/* Sets property in node. */
		node = malloc(sizeof(cc_arraylist_node));
		node->object = addobject;
		node->parent = properties;
		node->previous = properties->back;
		node->next = NULL;

		/* Adds node to an arraylist. */
		properties->length ++;
		if (properties->front == NULL)
		{
			/* first time. */
			properties->front = node;
			properties->back = node;
			properties->cursor = node;
		} else {
			properties->back->next = node;
			properties->back = node;
		}
		/* update cursor */
		properties->cursor = node;
		properties->cursor_index = properties->length - 1;
		result = properties->cursor_index;
	}
	return result;
}

int cc_arraylist_addAtCursor(cc_arraylist *list, cc_object *insertobject)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *node;
	cc_arraylist_node *insertnode;
	int result = -1;

	if (list != NULL && list->properties != NULL && insertobject != NULL) {
		properties = list->properties;
		insertnode = properties->cursor;
		if (insertnode != NULL) {
			properties->length ++;
			cc_object_grab(insertobject);
			node = malloc(sizeof(cc_arraylist_node));
			node->object = insertobject;
			node->parent = properties;
			node->previous = insertnode->previous;
			node->next = insertnode;
			if (insertnode->previous != NULL) {
				insertnode->previous->next = node;
			} else {
				properties->front = node;
			}
			insertnode->previous = node;
			/* update cursor */
			properties->cursor = node;
			result = properties->cursor_index;
		} else {
			result = cc_arraylist_addAtBack(list, insertobject);
		}
	}
	return result;
}

int cc_arraylist_addAt(cc_arraylist *list, cc_object *insertobject, int index)
{
	int result = -1;

	if (cc_arraylist_setCursorAt(list, index) >= 0) {
		result = cc_arraylist_addAtCursor(list, insertobject);
	}
	return result;
}

int cc_arraylist_addDynamicAt(cc_arraylist *list, cc_object *insertobject, int index)
{
	int result = -1;

	if (cc_arraylist_setCursorAt(list, index) >= 0) {
		result = cc_arraylist_addAtCursor(list, insertobject);
	} else if (index >= 0) {
		result = cc_arraylist_addAtBack(list, insertobject);
	} else if (index < 0) {
		result = cc_arraylist_addAtFront(list, insertobject);
	}
	return result;
}

int cc_arraylist_removeAtFront(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *removenode;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		removenode = properties->front;
		if (removenode != NULL) {
			properties->length --;
			cc_object_release(removenode->object);
			properties->front = removenode->next;
			if (removenode->next == NULL) {
				properties->back = NULL;
			} else {
				removenode->next->previous = NULL;
			}
			/* update cursor */
			properties->cursor = properties->front;
			properties->cursor_index = 0;
			free(removenode);
			result = properties->cursor_index;
		}
	}
	return result;
}

int cc_arraylist_removeAtBack(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *removenode;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		removenode = properties->back;
		if (removenode != NULL) {
			properties->length --;
			cc_object_release(removenode->object);
			properties->back = removenode->previous;
			if (removenode->previous == NULL) {
				properties->front = NULL;
			} else {
				removenode->previous->next = NULL;
			}
			/* update cursor */
			properties->cursor = properties->back;
			properties->cursor_index = properties->length -1;
			free(removenode);
			result = properties->cursor_index;
		}
	}
	return result;
}

int cc_arraylist_removeAtCursor(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_arraylist_node *removenode;
	int result = -1;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		removenode = properties->cursor;
		if (removenode != NULL) {
			properties->length --;
			cc_object_release(removenode->object);
			if (removenode->previous == NULL) {
				properties->front = removenode->next;
			} else {
				removenode->previous->next = removenode->next;
			}
			if (removenode->next == NULL) {
				properties->back = removenode->previous;
			} else {
				removenode->next->previous = removenode->previous;
			}
			/* update cursor */
			properties->cursor = removenode->next;
			free(removenode);
			result = properties->cursor_index;
		}
	}
	return result;
}

int cc_arraylist_removeAt(cc_arraylist *list, int index)
{
	int result = -1;
	
	if (cc_arraylist_setCursorAt(list, index) >= 0) {
		result = cc_arraylist_removeAtCursor(list);
	}
	return result;
}

int cc_arraylist_removeDynamicAt(cc_arraylist *list, int index)
{
	int result = -1;
	
	if (cc_arraylist_setCursorAt(list, index) >= 0) {
		result = cc_arraylist_removeAtCursor(list);
	} else if (index >= 0) {
		result = cc_arraylist_removeAtBack(list);
	} else if (index < 0) {
		result = cc_arraylist_removeAtFront(list);
	}
	return result;
}

cc_object *cc_arraylist_getAtFront(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->front != NULL) {
			result_object = properties->front->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getAtBack(cc_arraylist *list)
{
	cc_arraylist_properties *properties;
	cc_object *result_object = NULL;

	if (list != NULL && list->properties != NULL) {
		properties = list->properties;
		if (properties->back != NULL) {
			result_object = properties->back->object;
			cc_object_grab(result_object);
		}
	}
	return result_object;
}

cc_object *cc_arraylist_getAtCursor(cc_arraylist *list)
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

cc_object *cc_arraylist_getAt(cc_arraylist *list, int index)
{
	cc_arraylist_setCursorAt(list, index);
	return cc_arraylist_getAtCursor(list);
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
		if (properties->front != NULL) {
			/* Initializes cstring buffer. */
			cstring = malloc(add_alloc_size);
			memset(cstring, 0, add_alloc_size);
			cstring_length = 0;
			cstring_alloc_size = add_alloc_size;
			/* Builds cstring. */
			for (node = properties->front; node != NULL; node = node_next)
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

