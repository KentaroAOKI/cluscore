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

#include "cco_netEEngine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_netEEngine);

cco_netEEngine *cco_netEEngine_baseNew(int size)
{
	cco_netEEngine *o = NULL;
	do {
		if (size < sizeof(cco_netEEngine))
		{
			break;
		}
		o = (cco_netEEngine *)cco_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_netEEngine);
		cco_netEEngine_baseInitialize(o);
	} while (0);
	return o;
}

void cco_netEEngine_baseRelease(void *o)
{
	cco_netEEngine_baseFinalize(o);
	cco_baseRelease(o);
}

void cco_netEEngine_baseInitialize(cco_netEEngine *o)
{
	o->baseRelease = &cco_netEEngine_baseRelease;

	/*
	 * TODO: You should change code from default.
	 * It is constructor. You should change to following code and
	 * if you have additional valiable or function, write code to
	 * initialize class.
	 */
	o->cco_netEEngine_func = &cco_netEEngine_func;

	return;
}

void cco_netEEngine_baseFinalize(cco_netEEngine *o)
{
	/*
	 * TODO: You should change code from default.
	 * It is destructor. If you write additional code in
	 * cco_netEEngine_baseInitialize function,
	 * write code to finalize class.
	 */
	return;
}

cco_netEEngine *cco_netEEngine_new()
{
	return cco_netEEngine_baseNew(sizeof(cco_netEEngine));
}

void cco_netEEngine_release(void *o)
{
	cco_release(o);
}

/*
 * TODO: You should change code from default.
 * You should change to following code and if you have additional function,
 * write code to here.
 */
int cco_netEEngine_func(void *obj)
{
	return 0;
}
