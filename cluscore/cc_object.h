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

#ifndef CC_OBJECT_H
#define CC_OBJECT_H

#include <pthread.h>

#define CC_OBJECT_PROPERTIES \
	int *id;\
	int reference_count;\
	void *repleced_object;\
	pthread_mutex_t reference_count_mutex;\
	void (*cb_dispose_properties)(void *properties);\
	char *(*tocstring)(void *object);\
	int (*compare)(void *object1, void *object2);\
	int (*get_hashcode)(void *object, int salt);\
	void *properties;\

typedef struct cc_object_s
{
	CC_OBJECT_PROPERTIES
} cc_object;

#define cc_object_check_id(objectname, typename) objectname->id == &g_cc_##typename##_object_id

cc_object *cc_object_new(int *id, void *properties,
	void(*cb_dispose_properties));
cc_object *cc_object_new2(int *id, int properties_size,
	void(*cb_dispose_properties));
void cc_object_dispose(cc_object *object);
void cc_object_release(cc_object *object);
cc_object *cc_object_grab(cc_object *object);
char *cc_object_get_properties(cc_object *object);
void cc_object_replace(cc_object *replaced_object, cc_object *object);
int cc_object_equalsType(cc_object *object1, cc_object *object2);

#endif
