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

#include "cco_vAddrinfo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

cco_defineClass(cco_vAddrinfo);

cco_vAddrinfo *cco_vAddrinfo_baseNew(int size)
{
	cco_vAddrinfo *o = NULL;
	do {
		if (size < sizeof(cco_vAddrinfo))
		{
			break;
		}
		o = (cco_vAddrinfo *)cco_v_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_vAddrinfo);
		cco_vAddrinfo_baseInitialize(o);
	} while (0);
	return o;
}

void cco_vAddrinfo_baseRelease(void *o)
{
	cco_vAddrinfo_baseFinalize(o);
	cco_v_baseRelease(o);
}

void cco_vAddrinfo_baseInitialize(cco_vAddrinfo *o)
{
	o->baseRelease = &cco_vAddrinfo_baseRelease;
	o->v_getCstring = &cco_vAddrinfo_getCstring;
	o->v_compere = &cco_vAddrinfo_compere;
	o->v_hash = &cco_vAddrinfo_hash;
	return;
}

void cco_vAddrinfo_baseFinalize(cco_vAddrinfo *o)
{
	return;
}

cco_vAddrinfo *cco_vAddrinfo_new(struct addrinfo *ptr_addrinfo)
{
	return cco_vAddrinfo_baseNew(sizeof(cco_vAddrinfo));
}

cco_vAddrinfo *cco_vAddrinfo_newWithNameInCstring(char *hostname)
{
	return cco_vAddrinfo_new(NULL);
}

cco_vAddrinfo *cco_vAddrinfo_newWithNameInString(cco_vString *hostname)
{
	return cco_vAddrinfo_new(NULL);
}

void cco_vAddrinfo_release(void *o)
{
	cco_release(o);
}

char *cco_vAddrinfo_getCstring(void *o)
{
	return strdup("cco_v");
}

int cco_vAddrinfo_hash(void *obj, int salt)
{
	return salt;
}

int cco_vAddrinfo_compere(void *obj1, void *obj2)
{
	return obj1 - obj2;
}
