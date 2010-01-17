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
 * The cco_vBinary Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VBINARY_H_
#define CCO_VBINARY_H_

#include "cco_v.h"

#define CCO_VBINARY_PROPERTIES \
	int cco_vBinary;\
	int (*cco_vBinary_func)(void *cco_vBinary);

typedef struct cco_vBinary cco_vBinary;

struct cco_vBinary {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VBINARY_PROPERTIES
};

cco_vBinary *cco_vBinary_baseNew(int size);
void cco_vBinary_baseRelease(void *cco);
void cco_vBinary_baseInitialize(cco_vBinary *cco);
void cco_vBinary_baseFinalize(cco_vBinary *cco);
cco_vBinary *cco_vBinary_new();
void cco_vBinary_release(void *cco);
int cco_vBinary_func(void *cco_vBinary);

/* Don't touch following comment.
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_V_PROPERTIES
CCOINHERITANCE:CCO_VBINARY_PROPERTIES
*/

#endif /* CCO_VBINARY_H_ */
