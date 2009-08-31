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
 * The cco_vString Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VSTRING_H_
#define CCO_VSTRING_H_

#include "cco_v.h"

#define CCO_VSTRING_PROPERTIES \
	char *vString_cstring;\
	int vString_length;

typedef struct cco_vString cco_vString;

struct cco_vString {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VSTRING_PROPERTIES
};

cco_vString *cco_vString_baseNew(int size);
void cco_vString_baseRelease(void *cco);
void cco_vString_baseInitialize(cco_vString *cco);
void cco_vString_baseFinalize(cco_vString *cco);
cco_vString *cco_vString_new(char *cstring);
void cco_vString_release(void *cco);

char *cco_vString_getCstring(void *ccov_string);
int cco_vString_hash(void *ccov_string, int salt);
int cco_vString_compere(void *ccov_string1, void *ccov_string2);

#endif /* CCO_VSTRING_H_ */

/*
CCOPERTIES:CCO_PROPERTIES
CCOPERTIES:CCO_V_PROPERTIES
CCOPERTIES:CCO_VSTRING_PROPERTIES
*/
