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
 * The cco_vData Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VDATA_H_
#define CCO_VDATA_H_

#include "cco_v.h"

#define CCO_VDATA_PROPERTIES \
	int cco_vData;\
	int (*cco_vData_func)(void *cco_vData);

typedef struct cco_vData cco_vData;

struct cco_vData {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VDATA_PROPERTIES
};

cco_vData *cco_vData_baseNew(int size);
void cco_vData_baseRelease(void *cco);
void cco_vData_baseInitialize(cco_vData *cco);
void cco_vData_baseFinalize(cco_vData *cco);
cco_vData *cco_vData_new();
void cco_vData_release(void *cco);
int cco_vData_func(void *cco_vData);

#endif /* CCO_VDATA_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_V_PROPERTIES
CCOPERTIES:CCO_VDATA_PROPERTIES
*/
