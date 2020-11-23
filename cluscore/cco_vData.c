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

#include "cco_vData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_vData);

cco_vData *cco_vData_baseNew(int size)
{
	cco_vData *o = NULL;
	do {
		if (size < sizeof(cco_vData))
		{
			break;
		}
		o = (cco_vData *)cco_v_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_vData);
		cco_vData_baseInitialize(o);
	} while (0);
	return o;
}

void cco_vData_baseRelease(void *o)
{
	cco_vData_baseFinalize(o);
	cco_v_baseRelease(o);
}

void cco_vData_baseInitialize(cco_vData *o)
{
	o->baseRelease = &cco_vData_baseRelease;
	o->cco_vData_func = &cco_vData_func;
	return;
}

void cco_vData_baseFinalize(cco_vData *o)
{
	return;
}

cco_vData *cco_vData_new()
{
	return cco_vData_baseNew(sizeof(cco_vData));
}

void cco_vData_release(void *o)
{
	cco_release(o);
}

int cco_vData_func(void *obj)
{
	return 0;
}
