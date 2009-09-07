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
 * The cco_v Class for ClusCore.
 *
 * Author: Kentaro Aoki
 */

#include "cco_v.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_v);

cco_v *cco_v_baseNew(int size)
{
	cco_v *o;
	do {
		if (size < sizeof(cco_v))
		{
			break;
		}
		o = (cco_v *)cco_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_v);
		cco_v_baseInitialize(o);
	} while (0);
	return o;
}

void cco_v_baseRelease(void *o)
{
	cco_v_baseFinalize(o);
	cco_baseRelease(o);
}

void cco_v_baseInitialize(cco_v *o)
{
	o->baseRelease = &cco_v_baseRelease;
	o->v_getCstring = &cco_v_getCstring;
	o->v_compere = &cco_v_compere;
	o->v_hash = &cco_v_hash;
	return;
}

void cco_v_baseFinalize(cco_v *o)
{
	return;
}

cco_v *cco_v_new()
{
	return cco_v_baseNew(sizeof(cco_v));
}

void cco_v_release(void *o)
{
	cco_release(o);
}

char *cco_v_getCstring(void *o)
{
	return strdup("cco_v");
}

int cco_v_hash(void *obj, int salt)
{
	return salt;
}

int cco_v_compere(void *obj1, void *obj2)
{
	return obj1 - obj2;
}
