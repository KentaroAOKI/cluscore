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
 * The Object Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#include "cc_object.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef LIBCLUSCORE_DEBUG
pthread_mutex_t  cc_object_debug_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
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
#ifdef LIBCLUSCORE_DEBUG
		pthread_mutex_lock(&cc_object_debug_alloc_mutex);
		cc_object_debug_alloc_count++;
		pthread_mutex_unlock(&cc_object_debug_alloc_mutex);
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
#ifdef LIBCLUSCORE_DEBUG
		pthread_mutex_lock(&cc_object_debug_alloc_mutex);
		cc_object_debug_alloc_count++;
		pthread_mutex_unlock(&cc_object_debug_alloc_mutex);
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
#ifdef LIBCLUSCORE_DEBUG
		pthread_mutex_lock(&cc_object_debug_alloc_mutex);
		cc_object_debug_alloc_count--;
		if ((cc_object_debug_alloc_count%10) == 0) {
			printf("cc_object is %d.\n", cc_object_debug_alloc_count);
		}
		pthread_mutex_unlock(&cc_object_debug_alloc_mutex);
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

cc_object *cc_object_grab(cc_object *object)
{
	if (object != NULL)
	{
		pthread_mutex_lock(&object->reference_count_mutex);
		object->reference_count++;
		pthread_mutex_unlock(&object->reference_count_mutex);
	}
	return object;
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
