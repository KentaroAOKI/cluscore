/*
 * ClusCore
 * cc_container
 */
#ifndef CC_CONTAINER_H
#define CC_CONTAINER_H

#include "cc_object.h"

extern int g_cc_container_object_id;
typedef cc_object cc_container;
typedef struct cc_container_properties cc_container_properties;

struct cc_container_properties
{
	void *buffer;
	void (*cb_free)(void *buffer);
	int size;
};

cc_container *cc_container_new(void *buffer, int size, void (*cb_free_buffer));
void cc_container_dispose(cc_container *container);

#endif
