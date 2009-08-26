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
 * The Integer Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#include "cc_integer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* printf */

/*
 * Defines prototypes and Structures that it is private.
 */
typedef struct cc_integer_properties cc_integer_properties;

/** Defines a properties of a object for the cc_integer. */
struct cc_integer_properties
{
    int value;
};

/* Defines prototypes of the cc_integer for private. */
cc_integer_properties *cc_integer_properties_new(int value);
void cc_integer_properties_dispose(cc_integer_properties *properties);

/** Defines the Object-ID of int. */
int g_cc_integer_object_id;

/**
 * Allocates a properties of the cc_integer when dealt by cc_integer_new().
 * @return a properties of cc_integer.
 */
cc_integer_properties *cc_integer_properties_new(int value)
{
    cc_integer_properties *properties = NULL;
    do
    {
	/* creates cc_integer_properties */
	properties = malloc(sizeof (cc_integer_properties));
	if (properties == NULL)
	{
	    break;
	}
	/* sets values */
	properties->value = value;
    }
    while (0);
    return properties;
}

/**
 * Frees a properties of the cc_integer when dealt by cc_integer_release().
 * @properties a properties of cc_integer to free.
 */
void cc_integer_properties_dispose(cc_integer_properties *properties)
{
    if (properties != NULL)
    {
	/* disposes cc_integer_properties */
	free(properties);
    }
    return;
}

/**
 * Creates a object of the cc_integer.
 * @return a object of the cc_integer.
 */
cc_integer *cc_integer_new(int value)
{
    cc_integer *object = NULL;
    cc_integer_properties *properties = cc_integer_properties_new(value);
    object = cc_object_new(&g_cc_integer_object_id, properties, cc_integer_properties_dispose);
    /* sets callback functions. */
    object->tocstring = (void *) cc_integer_tocstring;
    object->compare = (void *) cc_integer_compere;
    return (object);
}

/*
 * Releases a object of the cc_integer.
 * @tree a object of cc_integer to free.
 */
void cc_integer_release(cc_integer *object)
{
    cc_object_release(object);
    return;
}

int cc_integer_get(cc_integer *object, int *out_value)
{
    cc_integer_properties *properties;
    properties = object->properties;
    *out_value = properties->value;
    return *out_value;
}

cc_integer *cc_integer_set(cc_integer *object, int value)
{
    cc_integer_properties *properties;
    properties = object->properties;
    properties->value = value;
    return object;
}

cc_integer *cc_integer_plus(cc_integer *plused_object, cc_integer *object)
{
    cc_integer_properties *plused_properties;
    cc_integer_properties *properties;
    plused_properties = plused_object->properties;
    properties = object->properties;
    plused_properties->value += properties->value;
    return plused_object;
}

cc_integer *cc_integer_minus(cc_integer *minused_object, cc_integer *object)
{
    cc_integer_properties *plused_properties;
    cc_integer_properties *properties;
    plused_properties = minused_object->properties;
    properties = object->properties;
    plused_properties->value -= properties->value;
    return minused_object;
}

char *cc_integer_tocstring(cc_integer *object)
{
    cc_integer_properties *properties;
    char *cstring;
    properties = object->properties;
    cstring = malloc(16);
    snprintf(cstring, 16, "%d", properties->value);
    return cstring;
}

int cc_integer_compere(cc_integer *object1, cc_integer *object2)
{
    cc_integer_properties *properties1;
    cc_integer_properties *properties2;
    properties1 = object1->properties;
    properties2 = object2->properties;
    return properties1->value - properties2->value;
}
