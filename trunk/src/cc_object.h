/*
 * ClusCore
 * cc_object
 */

#ifndef CC_OBJECT_H
#define CC_OBJECT_H

#define CC_OBJECT_PROPERTIES \
	int *id;\
	void *properties;\
	int reference_count;\
	void *repleced_object;\
	void (*cb_dispose_properties)(void *properties);\
	char *(*tocstring)(void *object);\
	char *(*compare)(void *object);

typedef struct cc_object_s {
	CC_OBJECT_PROPERTIES
} cc_object;

#define check_object_id(objectname, typename) objectname->id == &g_cc_##typename##_object_id

cc_object *cc_object_new(int *id, void *properties, void (*cb_dispose_properties));
void cc_object_dispose(cc_object *object);
void cc_object_release(cc_object *object);
void cc_object_grab(cc_object *object);
char *cc_object_get_properties(cc_object *object);

#endif
