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

#include "cco_vString.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(vString);

cco_vString *cco_vString_baseNew(int size)
{
	cco_vString *o = NULL;
	do {
		if (size < sizeof(cco_vString))
		{
			break;
		}
		o = (cco_vString *)cco_v_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, vString);
		cco_vString_baseInitialize(o);
	} while (0);
	return o;
}

void cco_vString_baseRelease(void *o)
{
	cco_vString_baseFinalize(o);
	cco_v_baseRelease(o);
}

void cco_vString_baseInitialize(cco_vString *o)
{
	o->baseRelease = &cco_vString_baseRelease;
	o->v_getCstring = &cco_vString_getCstring;
	o->v_compere = &cco_vString_compere;
	o->v_hash = &cco_vString_hash;
	o->vString_cstring = NULL;
	o->vString_length = 0;
	return;
}

void cco_vString_baseFinalize(cco_vString *o)
{
	if (o->vString_cstring != NULL)
	{
		free(o->vString_cstring);
	}
	return;
}

cco_vString *cco_vString_new(char *cstring)
{
	cco_vString *o;
	o = cco_vString_baseNew(sizeof(cco_vString));
	if (o != NULL)
	{
		o->vString_cstring = strdup(cstring);
		o->vString_length = strlen(cstring);
	}
	return o;
}

void cco_vString_release(void *o)
{
	cco_release(o);
}

char *cco_vString_getCstring(void *o)
{
	return strdup(((cco_vString *)o)->vString_cstring);
}

int cco_vString_hash(void *o, int salt)
{
	char *cstr_cursor;
	int hashvalue;

	cstr_cursor = ((cco_vString *)o)->vString_cstring;
	hashvalue = salt;
	while (*cstr_cursor++)
	{
		hashvalue = hashvalue * 37 + (unsigned char) *cstr_cursor;
	}
	return(hashvalue);
}

int cco_vString_compere(void *o1, void *o2)
{
	int result;
	result = strcmp(((cco_vString *)o1)->vString_cstring,
			((cco_vString *)o2)->vString_cstring);
	return (result);
}
