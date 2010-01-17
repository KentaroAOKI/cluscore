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

#include "cco_vBinary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_vBinary);

cco_vBinary *cco_vBinary_baseNew(int size)
{
	cco_vBinary *o = NULL;
	do {
		if (size < sizeof(cco_vBinary))
		{
			break;
		}
		o = (cco_vBinary *)cco_v_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_vBinary);
		cco_vBinary_baseInitialize(o);
	} while (0);
	return o;
}

void cco_vBinary_baseRelease(void *o)
{
	cco_vBinary_baseFinalize(o);
	cco_v_baseRelease(o);
}

void cco_vBinary_baseInitialize(cco_vBinary *o)
{
	o->baseRelease = &cco_vBinary_baseRelease;
	o->cco_vBinary_func = &cco_vBinary_func;
	return;
}

void cco_vBinary_baseFinalize(cco_vBinary *o)
{
	return;
}

cco_vBinary *cco_vBinary_new()
{
	return cco_vBinary_baseNew(sizeof(cco_vBinary));
}

void cco_vBinary_release(void *o)
{
	cco_release(o);
}

int cco_vBinary_func(void *obj)
{
	return 0;
}
