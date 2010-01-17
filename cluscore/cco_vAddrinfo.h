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
 * The cco_vAddrinfo Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VADDRINFO_H_
#define CCO_VADDRINFO_H_

#include "cco_v.h"
#include "cco_vString.h"
#include <sys/socket.h>
#include <netdb.h>

#define CCO_VADDRINFO_PROPERTIES \
	struct addrinfo vAddrinfo_addrinfo;

typedef struct cco_vAddrinfo cco_vAddrinfo;

struct cco_vAddrinfo {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VADDRINFO_PROPERTIES
};

cco_vAddrinfo *cco_vAddrinfo_baseNew(int size);
void cco_vAddrinfo_baseRelease(void *cco);
void cco_vAddrinfo_baseInitialize(cco_vAddrinfo *cco);
void cco_vAddrinfo_baseFinalize(cco_vAddrinfo *cco);
cco_vAddrinfo *cco_vAddrinfo_new(struct addrinfo *ptr_addrinfo);
cco_vAddrinfo *cco_vAddrinfo_newWithNameInCstring(char *hostname);
cco_vAddrinfo *cco_vAddrinfo_newWithNameInString(cco_vString *hostname);
void cco_vAddrinfo_release(void *cco);
char *cco_vAddrinfo_getCstring(void *o);
int cco_vAddrinfo_hash(void *obj, int salt);
int cco_vAddrinfo_compere(void *obj1, void *obj2);

/* Don't touch following comment.
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_V_PROPERTIES
CCOINHERITANCE:CCO_VADDRINFO_PROPERTIES
*/

#endif /* CCO_VADDRINFO_H_ */
