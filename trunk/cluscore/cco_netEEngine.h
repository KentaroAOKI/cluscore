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
 * The cco_netEEngine Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_NETEENGINE_H_
#define CCO_NETEENGINE_H_

#include "cco.h"

/*
 * TODO: You should change code from default.
 * You should change to following code and if you have additional variable
 * or function, write code to here.
 */
#define CCO_NETEENGINE_PROPERTIES \
	int cco_netEEngine;\
	int (*cco_netEEngine_func)(void *cco_netEEngine);

typedef struct cco_netEEngine cco_netEEngine;

struct cco_netEEngine {
	CCO_PROPERTIES
	CCO_NETEENGINE_PROPERTIES
};

cco_netEEngine *cco_netEEngine_baseNew(int size);
void cco_netEEngine_baseRelease(void *cco);
void cco_netEEngine_baseInitialize(cco_netEEngine *cco);
void cco_netEEngine_baseFinalize(cco_netEEngine *cco);
cco_netEEngine *cco_netEEngine_new();
void cco_netEEngine_release(void *cco);

/*
 * TODO: You should change code from default.
 * You should change to following code and if you have additional function,
 * write code to here.
 */
int cco_netEEngine_func(void *cco_netEEngine);

/* Don't touch following comment.
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_NETEENGINE_PROPERTIES
*/

#endif /* CCO_NETEENGINE_H_ */
