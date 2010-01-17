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
 * The cco_vSocket Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VSOCKET_H_
#define CCO_VSOCKET_H_

#include "cco_v.h"

/*
 * TODO: You should change code from default.
 * You should change to following code and if you have additional variable
 * or function, write code to here.
 */
#define CCO_VSOCKET_PROPERTIES \
	int cco_vSocket;\
	int (*cco_vSocket_func)(void *cco_vSocket);

typedef struct cco_vSocket cco_vSocket;

struct cco_vSocket {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VSOCKET_PROPERTIES
};

cco_vSocket *cco_vSocket_baseNew(int size);
void cco_vSocket_baseRelease(void *cco);
void cco_vSocket_baseInitialize(cco_vSocket *cco);
void cco_vSocket_baseFinalize(cco_vSocket *cco);
cco_vSocket *cco_vSocket_new();
void cco_vSocket_release(void *cco);

/*
 * TODO: You should change code from default.
 * You should change to following code and if you have additional function,
 * write code to here.
 */
int cco_vSocket_func(void *cco_vSocket);

/* Don't touch following comment.
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_V_PROPERTIES
CCOINHERITANCE:CCO_VSOCKET_PROPERTIES
*/

#endif /* CCO_VSOCKET_H_ */
