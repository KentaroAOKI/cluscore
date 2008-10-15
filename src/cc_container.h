/*
 * ClusCore
 * cc_container
 */
#ifndef CC_CONTAINER_H
#define CC_CONTAINER_H

#include "cc_object.h"

extern int g_cc_container_object_id;
typedef cc_object cc_container;

cc_container *cc_container_new(void *buffer, void (*cb_free_buffer));
void cc_container_dispose(cc_container *container);
void cc_container_setTocstring(cc_container *container, char *(*tocstring));
void cc_container_setCompare(cc_container *container, int (*compare));
char *cc_container_tocstring(cc_container *container);
int cc_container_compare(cc_container *container1, cc_container *container2);

#endif
