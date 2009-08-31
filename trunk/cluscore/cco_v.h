/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *  Copyright (c) 2009 ClusCore
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
 * The cco_v Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_V_H_
#define CCO_V_H_

#include "cco.h"

#define CCO_V_PROPERTIES \
	int (*v_compere)(void *cco_v1, void *cco_v2);\
	int (*v_hash)(void *cco_v, int salt);\
	char *(*v_getCstring)(void *cco_v);

typedef struct cco_v cco_v;

struct cco_v {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
};

cco_v *cco_v_baseNew(int size);
void cco_v_baseRelease(void *cco);
void cco_v_baseInitialize(cco_v *cco);
void cco_v_baseFinalize(cco_v *cco);
cco_v *cco_v_new();
void cco_v_release(void *cco);

char *cco_v_getCstring(void *o);
int cco_v_hash(void *obj, int salt);
int cco_v_compere(void *obj1, void *obj2);

#endif /* CCO_V_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_V_PROPERTIES
*/
