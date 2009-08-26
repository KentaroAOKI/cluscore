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

#ifndef CC_CONTAINER_H
#define CC_CONTAINER_H

#include "cc_object.h"

extern int g_cc_container_object_id;
typedef cc_object cc_container;

cc_container *cc_container_new(void *buffer, void(*cb_free_buffer));
void cc_container_release(cc_container *container);
void cc_container_setTocstring(cc_container *container, char *(*tocstring));
void cc_container_setCompare(cc_container *container, int(*compare));
void *cc_container_getrefBuffer(cc_container *container);
char *cc_container_tocstring(cc_container *container);
int cc_container_compare(cc_container *container1, cc_container *container2);

#endif
