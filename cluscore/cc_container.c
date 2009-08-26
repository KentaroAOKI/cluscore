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
 * The Container Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#include "cc_container.h"
#include <stdlib.h>
#include <string.h>

typedef struct cc_container_properties cc_container_properties;

struct cc_container_properties
{
	void *buffer;
	void (*cb_free)(void *buffer);
	char *(*tocstring)(void *buffer);
	int (*compare)(void *buffer1, void *buffer2);
};

int g_cc_container_object_id;

cc_container_properties *cc_container_properties_new(void *buffer,
		void(*cb_free_buffer))
{
	cc_container_properties *properties;
	properties = malloc(sizeof(cc_container_properties));
	if (properties != NULL)
	{
		memset(properties, 0, sizeof(cc_container_properties));
		properties->buffer = buffer;
		properties->cb_free = cb_free_buffer;
	}
	return properties;
}

void cc_container_properties_dispose(cc_container_properties *properties)
{
	if (properties != NULL)
	{
		if (properties->cb_free != NULL)
		{
			properties->cb_free(properties->buffer);
		}
		free(properties);
	}
	return;
}

cc_container *cc_container_new(void *buffer, void(*cb_free_buffer))
{
	cc_container *object = NULL;
	cc_container_properties *properties = cc_container_properties_new(buffer,
			cb_free_buffer);
	object = cc_object_new(&g_cc_container_object_id, properties,
			cc_container_properties_dispose);
	object->tocstring = (void *) cc_container_tocstring;
	object->compare = (void *) cc_container_compare;
	return (object);
}

void cc_container_release(cc_container *container)
{
	cc_object_release(container);
	return;
}

void *cc_container_getrefBuffer(cc_container *container)
{
	void *result = NULL;
	cc_container_properties *properties;

	if (container != NULL)
	{
		properties = container->properties;
		result = properties->buffer;
	}
	return result;
}

void cc_container_setTocstring(cc_container *container, char *(*tocstring))
{
	cc_container_properties *properties;
	if (container != NULL)
	{
		properties = container->properties;
		properties->tocstring = (void *) tocstring;
	}
	return;
}

void cc_container_setCompare(cc_container *container, int(*compare))
{
	cc_container_properties *properties;
	if (container != NULL)
	{
		properties = container->properties;
		properties->compare = (void *) compare;
	}
	return;
}

char *cc_container_tocstring(cc_container *container)
{
	cc_container_properties *properties = container->properties;
	return properties->tocstring(properties->buffer);
}

int cc_container_compare(cc_container *container1, cc_container *container2)
{
	cc_container_properties *properties1;
	cc_container_properties *properties2;
	int result = -1;
	if (container1 != NULL && container2 != NULL
			&& container1->cb_dispose_properties
					== container2->cb_dispose_properties)
	{
		properties1 = container1->properties;
		properties2 = container2->properties;
		result = properties1->compare(properties1->buffer, properties2->buffer);
	}
	return result;
}
