/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *
 *  http://www.cluscore.com/
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
 * The ClusCore.
 *
 * Author: Kentaro Aoki
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cco.h"

#ifdef LIBCLUSCORE_DEBUG
pthread_mutex_t  cco_baseDebugallocmutex = PTHREAD_MUTEX_INITIALIZER;
int cco_baseDebugalloccount = 0;
#endif

int g_cco_baseId;

cco *cco_baseNew(int size)
{
	cco *o;
	do {
		if (size < sizeof(cco)) {
			break;
		}
		o = malloc(size);
		if (o == NULL) {
			break;
		}
		cco_baseInitialize(o);
#ifdef LIBCLUSCORE_DEBUG
		pthread_mutex_lock(&cco_baseDebugallocmutex);
		cco_baseDebugalloccount ++;
		pthread_mutex_unlock(&cco_baseDebugallocmutex);
#endif
		} while (0);
	return o;
}

void cco_baseRelease(void *o)
{
	cco_baseFinalize(o);
	free(o);
#ifdef LIBCLUSCORE_DEBUG
	pthread_mutex_lock(&cco_baseDebugallocmutex);
	cco_baseDebugalloccount --;
	pthread_mutex_unlock(&cco_baseDebugallocmutex);
#endif
	return;
}

void cco_baseInitialize(cco *o)
{
	o->baseReferencecount = 1;
#if HAVE_PTHREAD_H
	pthread_mutex_init(&o->baseReferencecountmutex, NULL);
#endif
	o->baseRelease = &cco_baseRelease;
	o->getCstring = &cco_baseGetCstring;
	o->compare = &cco_compare;
	return;
}

void cco_baseFinalize(cco *o)
{
	return;
}

char *cco_baseGetCstring(void *o)
{
	return strdup("cco");
}

int cco_compare(void *o1, void *o2)
{
	return o1 - o2;
}

cco *cco_new()
{
	return cco_baseNew(sizeof(cco));
}

void cco_release(void *o)
{
	int count;

	if (o != NULL) {
#if HAVE_PTHREAD_H
		pthread_mutex_lock(&((cco*)o)->baseReferencecountmutex);
#endif
		((cco*)o)->baseReferencecount --;
		count = ((cco*)o)->baseReferencecount;
#if HAVE_PTHREAD_H
		pthread_mutex_unlock(&((cco*)o)->baseReferencecountmutex);
#endif
		if (count <= 0) {
			((cco*)o)->baseRelease(o);
		}
	}
	return;
}

void cco_grab(void *o)
{
	int count;

	if (o != NULL) {
#if HAVE_PTHREAD_H
		pthread_mutex_lock(&((cco*)o)->baseReferencecountmutex);
#endif
		((cco*)o)->baseReferencecount ++;
		count = ((cco*)o)->baseReferencecount;
#if HAVE_PTHREAD_H
		pthread_mutex_unlock(&((cco*)o)->baseReferencecountmutex);
#endif
	}
	return;
}
