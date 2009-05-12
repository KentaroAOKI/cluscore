/*
 * ClusCore
 * cc_object
 */

#include "cc_object.h"
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
int cc_object_debug_alloc_count = 0;
#endif

cc_object *cc_object_new(int *id, void *properties,
		void(*cb_dispose_properties))
{
	cc_object *object = NULL;
	void *object_ptr;
	object = malloc(sizeof(cc_object));
	if (object != NULL)
	{
		object_ptr = object;
		object->id = id;
		object->reference_count = 1;
		pthread_mutex_init(&object->reference_count_mutex, NULL);
		object->repleced_object = NULL;
		object->cb_dispose_properties = cb_dispose_properties;
		object->properties = properties;
#ifdef DEBUG
		cc_object_debug_alloc_count++;
#endif
	}
	return object;
}

cc_object *cc_object_new2(int *id, int properties_size,
		void(*cb_dispose_properties))
{
	cc_object *object = NULL;
	object = malloc(sizeof(cc_object) + properties_size);
	if (object != NULL)
	{
		object->id = id;
		object->properties = (char *) object + sizeof(cc_object);
		object->repleced_object = NULL;
		object->cb_dispose_properties = cb_dispose_properties;
		object->reference_count = 1;
#ifdef DEBUG
		cc_object_debug_alloc_count++;
#endif
	}
	return object;
}

void cc_object_dispose(cc_object *object)
{
	if (object != NULL)
	{
		if (object->properties != NULL && object->cb_dispose_properties != NULL)
		{
			object->cb_dispose_properties(object->properties);
		}
		if (object->repleced_object != NULL)
		{
			cc_object_release(object->repleced_object);
		}
		free(object);
#ifdef DEBUG
		cc_object_debug_alloc_count--;
#endif
	}
	return;
}

void cc_object_release(cc_object *object)
{
	int count;
	if (object != NULL)
	{
		pthread_mutex_lock(&object->reference_count_mutex);
		object->reference_count--;
		count = object->reference_count;
		pthread_mutex_unlock(&object->reference_count_mutex);
		if (count <= 0)
		{
			cc_object_dispose(object);
		}
	}
	return;
}

void cc_object_grab(cc_object *object)
{
	if (object != NULL)
	{
		object->reference_count++;
	}
	return;
}

char *cc_object_get_properties(cc_object *object)
{
	return object->properties;
}

void cc_object_replace(cc_object *replaced_object, cc_object *object)
{
	if (replaced_object->id == object->id)
	{
		if (replaced_object->repleced_object != NULL)
		{
			cc_object_release(replaced_object->repleced_object);
		}
		replaced_object->cb_dispose_properties(object->properties);
		replaced_object->properties = object->properties;
		replaced_object->repleced_object = object;
		cc_object_grab(object);
	}
	return;
}

int cc_object_equalsType(cc_object *object1, cc_object *object2)
{
	int result = -1;
	if (object1 != NULL && object2 != NULL && object1->id == object2->id)
	{
		result = 0;
	}
	return result;
}
