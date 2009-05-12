/*
 * ClusCore
 * cc_object
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
void cc_object_grab(cc_object *object);
char *cc_object_get_properties(cc_object *object);
void cc_object_replace(cc_object *replaced_object, cc_object *object);
int cc_object_equalsType(cc_object *object1, cc_object *object2);

#endif
