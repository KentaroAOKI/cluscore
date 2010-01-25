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

#include <stdarg.h>
#include "cco_arraylist.h"
#include "cco_v.h"

#define CCO_VSTRING_PROPERTIES \
	char *vString_cstring;\
	cco_arraylist *vString_otherstrings;\
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
cco_vString *cco_vString_newWithLength(char *cstring, int length);
cco_vString *cco_vString_newWithFormat(char *cstring_format, ...);
cco_vString *cco_vString_newWithVformat(char *cstring_format, va_list ap);
void cco_vString_release(void *cco);

char *cco_vString_getCstring(void *ccov_string);
int cco_vString_hash(void *ccov_string, int salt);
int cco_vString_compere(void *ccov_string1, void *ccov_string2);

int cco_vString_length(cco_vString *o);
void cco_vString_catenate(cco_vString *base_o, cco_vString *o);
void cco_vString_catenateWithFormat(cco_vString *base_o, char *cstring_format, ...);
cco_vString *cco_vString_getReplasedString(cco_vString *string, char *target_regex,	cco_vString *replase_string);
cco_vString *cco_vString_getReplasedStringWithCstring(cco_vString *string, char *target_regex, char *replase_cstring);

#endif /* CCO_VSTRING_H_ */

/*
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_V_PROPERTIES
CCOINHERITANCE:CCO_VSTRING_PROPERTIES
*/
