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

#ifndef CCO_H_
#define CCO_H_

#include "config.h"

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif /* HAVE_PTHREAD_H */

#if HAVE_PTHREAD_H
#define CCO_PROPERTIES \
	int *baseId;\
	int *baseSuperId;\
	int baseReferencecount;\
	pthread_mutex_t baseReferencecountmutex;\
	void (*baseRelease)(void *o);
#else
#define CCO_PROPERTIES \
	int *baseId;\
	int *baseSuperId;\
	int baseReferencecount;\
	void (*baseRelease)(void *o);
#endif /* HAVE_PTHREAD_H */

typedef struct cco cco;
struct cco {
	CCO_PROPERTIES
};

#define cco_defineClass(NAME) int g_##NAME##_baseId;
#define cco_defineExternClass(NAME) extern int g_##NAME##_baseId;
#define cco_setClass(CCO, NAME) ((cco*)CCO)->baseSuperId = ((cco*)CCO)->baseId;((cco*)CCO)->baseId = &g_##NAME##_baseId;
#define cco_compareClass(CCO, NAME) (((cco*)CCO)->baseId == &g_##NAME##_baseId)
#define cco_compareSuperClass(CCO, NAME) (((cco*)CCO)->baseSuperId == &g_##NAME##_baseId)

cco *cco_baseNew(int size);
void cco_baseRelease(void *o);
void cco_baseInitialize(cco *o);
void cco_baseFinalize(cco *o);
int cco_compare(void *o1, void *o2);
cco *cco_new();
void cco_release(void *o);
void cco_safeRelease(void **o);
void cco_grab(void *o);
void *cco_get(void *o);

#endif /* CCO_H_ */

/*
CCOINHERITANCE:CCO_PROPERTIES
*/
