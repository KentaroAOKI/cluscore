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
#include <regex.h> /* regcomp */

static char *cco_vString_format_sub_itoa(char *buf, int buflen, int i);
cco_defineExternClass(cco_v);

cco_defineClass(cco_vString);

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
		cco_setClass(o, cco_vString);
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
	o->vString_otherstrings = NULL;
	o->vString_length = 0;
	return;
}

void cco_vString_baseFinalize(cco_vString *o)
{
	if (o->vString_cstring != NULL)
	{
		free(o->vString_cstring);
		cco_release(o->vString_otherstrings);
	}
	return;
}

cco_vString *cco_vString_newWithLength(char *cstring, int length)
{
	cco_vString *o = NULL;
	do {
		if (cstring == NULL)
		{
			break;
		}
		o = cco_vString_baseNew(sizeof(cco_vString));
		if (o == NULL)
		{
			break;
		}
		o->vString_cstring = malloc(length + 1);
		memcpy(o->vString_cstring, cstring, length);
		o->vString_cstring[length] = 0;
		o->vString_length = length;
	} while (0);
	return o;
}

cco_vString *cco_vString_new(char *cstring)
{
	cco_vString *o = NULL;
	do {
		if (cstring == NULL)
		{
			break;
		}
		o = cco_vString_newWithLength(cstring, strlen(cstring));
	} while (0);
	return o;
}
#if 0
cco_vString *cco_vString_newWithVformat(char *cstring_format, va_list ap)
{
	regex_t preg;
	regmatch_t pmatch[1];
	cco_vString *work_string;
	cco_vString *arg_string;
	int arg_int;
	char *arg_cstr;
	char arg_cstr_buff[256];
	cco *arg_cco;
	cco_vString *o = NULL;

	do {
		o = cco_vString_newWithLength("", 0);
		if (o == NULL)
		{
			break;
		}
		if (cstring_format == NULL)
		{
			break;
		}
		if (regcomp(&preg, "%[@ds]", REG_EXTENDED) != 0)
		{
			break;
		}
		while (regexec(&preg, cstring_format, 1, pmatch, 0) == 0)
		{
			work_string = cco_vString_newWithLength(cstring_format, pmatch[0].rm_so);
			cco_vString_catenate(o, work_string);
			switch (cstring_format[pmatch[0].rm_so + 1])
			{
				case '@':
					arg_cco = va_arg(ap, cco *);
					if (cco_compareClass(arg_cco, cco_vString))
					{
						cco_vString_catenate(o, (cco_vString*)arg_cco);
					}
					else if (cco_compareSuperClass(o, cco_v))
					{
						/* if a cco of argument is ex, to change a cco_vString. */
						arg_cstr = ((cco_v*)arg_cco)->v_getCstring(arg_cco);
						arg_string = cco_vString_new(arg_cstr);
						cco_vString_catenate(o, arg_string);
						cco_release(arg_string);
						free(arg_cstr);
					}
					break;
				case 'd':
					arg_int = va_arg(ap, int);
					arg_string = cco_vString_new(
							cco_vString_format_sub_itoa(arg_cstr_buff, sizeof(arg_cstr_buff), arg_int));
					cco_vString_catenate(o, arg_string);
					cco_release(arg_string);
					break;
				case 's':
					arg_cstr = va_arg(ap, char *);
					arg_string = cco_vString_new(arg_cstr);
					cco_vString_catenate(o, arg_string);
					cco_release(arg_string);
					break;
			}
			cco_release(work_string);
			cstring_format = cstring_format + pmatch[0].rm_eo;
		}
		work_string = cco_vString_new(cstring_format);
		cco_vString_catenate(o, work_string);
		cco_release(work_string);
		regfree(&preg);
	} while (0);
	return o;
}
#else
cco_vString *cco_vString_newWithVformat(char *cstring_format, va_list ap)
{
	cco_vString *work_string;
	cco_vString *arg_string;
	int arg_int;
	char *arg_cstr;
	char arg_cstr_buff[256];
	cco *arg_cco;
	cco_vString *o = NULL;

	int node_cstring_format_len;
	char *node_cstring_format;
	char *ptr_cstring_format;

	do {
		o = cco_vString_newWithLength("", 0);
		if (o == NULL)
		{
			break;
		}
		if (cstring_format == NULL)
		{
			break;
		}
		node_cstring_format_len = 0;
		node_cstring_format = cstring_format;
		ptr_cstring_format = cstring_format;
		while (*ptr_cstring_format != 0)
		{
			if (*ptr_cstring_format == '%')
			{
				*ptr_cstring_format ++;
				switch (*ptr_cstring_format)
				{
				case 'd':
					work_string = cco_vString_newWithLength(node_cstring_format, node_cstring_format_len);
					cco_vString_catenate(o, work_string);
					cco_release(work_string);
					node_cstring_format = ptr_cstring_format + 1;
					node_cstring_format_len = 0;

					arg_int = va_arg(ap, int);
					arg_string = cco_vString_new(
							cco_vString_format_sub_itoa(arg_cstr_buff, sizeof(arg_cstr_buff), arg_int));
					cco_vString_catenate(o, arg_string);
					cco_release(arg_string);
					break;
				case 's':
					work_string = cco_vString_newWithLength(node_cstring_format, node_cstring_format_len);
					cco_vString_catenate(o, work_string);
					cco_release(work_string);
					node_cstring_format = ptr_cstring_format + 1;
					node_cstring_format_len = 0;

					arg_cstr = va_arg(ap, char *);
					arg_string = cco_vString_new(arg_cstr);
					cco_vString_catenate(o, arg_string);
					cco_release(arg_string);
					break;
				case '@':
					work_string = cco_vString_newWithLength(node_cstring_format, node_cstring_format_len);
					cco_vString_catenate(o, work_string);
					cco_release(work_string);
					node_cstring_format = ptr_cstring_format + 1;
					node_cstring_format_len = 0;

					arg_cco = va_arg(ap, cco *);
					if (cco_compareClass(arg_cco, cco_vString))
					{
						cco_vString_catenate(o, (cco_vString*)arg_cco);
					}
					else if (cco_compareSuperClass(o, cco_v))
					{
						/* if a cco of argument is ex, to change a cco_vString. */
						arg_cstr = ((cco_v*)arg_cco)->v_getCstring(arg_cco);
						arg_string = cco_vString_new(arg_cstr);
						cco_vString_catenate(o, arg_string);
						cco_release(arg_string);
						free(arg_cstr);
					}
					break;
				case 0:
					continue;
					break;
				case '%':
					node_cstring_format_len ++;
					break;
				default:
					node_cstring_format_len ++;
					break;
				}
			} else {
				node_cstring_format_len ++;
			}
			ptr_cstring_format ++;
		}
		work_string = cco_vString_new(node_cstring_format);
		cco_vString_catenate(o, work_string);
		cco_release(work_string);
	} while (0);
	return o;
}
#endif
cco_vString *cco_vString_newWithFormat(char *cstring_format, ...)
{
	cco_vString *o;
	va_list ap;
	va_start(ap, cstring_format);
	o = cco_vString_newWithVformat(cstring_format, ap);
	va_end(ap);
	return o;
}

void cco_vString_release(void *o)
{
	cco_release(o);
}

char *cco_vString_getCstring(void *o)
{
	cco_vString *string = (cco_vString*)o;
	int size;
	cco_vString *work_string;
	char *work_cstring;
	char *cstring;
	char *cstring_ptr;
	do {
		if (string == NULL || !cco_compareClass(string, cco_vString))
		{
			break;
		}
		size = cco_vString_length(string) + 1;
//		size = string->vString_length + 1;
		cstring = malloc(size);
		if (cstring == NULL)
		{
			break;
		}
		cstring_ptr = cstring;
		if (string->vString_cstring != NULL)
		{
			size = strlen(string->vString_cstring);
			memcpy(cstring_ptr, string->vString_cstring, size);
			cstring_ptr += size;
		}
		if (string->vString_otherstrings != NULL)
		{
			cco_arraylist_setCursorAtFront(string->vString_otherstrings);
			work_string = (cco_vString *)cco_arraylist_getAtCursor(string->vString_otherstrings);
			while (work_string != NULL)
			{
				work_cstring = cco_vString_getCstring(work_string);
				size = cco_vString_length(work_string);
				memcpy(cstring_ptr, work_cstring, size);
				cstring_ptr += size;
				free(work_cstring);
				cco_release(work_string);
				cco_arraylist_setCursorAtNext(string->vString_otherstrings);
				work_string = (cco_vString *)cco_arraylist_getAtCursor(string->vString_otherstrings);
			}
		}
		*cstring_ptr = 0;
	} while (0);
	return cstring;
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
	char *o1_cstring;
	int o1_cstring_free;
	char *o2_cstring;
	int o2_cstring_free;

	do {
		/*
		 * TODO: checks inheritanceId
		 */
		if (o1 == NULL)
		{
			result = -1;
			break;
		}
		if (o2 == NULL)
		{
			result = 1;
			break;
		}
		if (cco_compareClass(o1, cco_vString) && cco_arraylist_length(((cco_vString *)o1)->vString_otherstrings) ==0)
		{
			o1_cstring = ((cco_vString *)o1)->vString_cstring;
			o1_cstring_free = 0;
		} else {
			o1_cstring = ((cco_v *)o1)->v_getCstring(o1);
			o1_cstring_free = 1;
		}
		if (cco_compareClass(o2, cco_vString) &&cco_arraylist_length(((cco_vString *)o2)->vString_otherstrings) ==0)
		{
			o2_cstring = ((cco_vString *)o2)->vString_cstring;
			o2_cstring_free = 0;
		} else {
			o2_cstring = ((cco_v *)o2)->v_getCstring(o2);
			o2_cstring_free = 1;
		}
		result = strcmp(o1_cstring, o2_cstring);
		if (o1_cstring_free) {
			free(o1_cstring);
		}
		if (o2_cstring_free) {
			free(o2_cstring);
		}
	} while (0);
	return (result);
}

int cco_vString_length(cco_vString *o)
{
	int result = 0;
	if (o != NULL)
	{
		result = o->vString_length;
	}
	return result;
}

void cco_vString_catenate(cco_vString *base_o, cco_vString *o)
{
	do {
		if (base_o == NULL || o == NULL)
		{
			break;
		}
		if (base_o->vString_otherstrings == NULL)
		{
			base_o->vString_otherstrings = cco_arraylist_new();
		}
		cco_arraylist_addAtBack(base_o->vString_otherstrings, o);
		base_o->vString_length += o->vString_length;
	} while (0);
	return;
}

void cco_vString_catenateWithFormat(cco_vString *base_o, char *cstring_format, ...)
{
	cco_vString *o;
	va_list ap;
	do {
		if (cstring_format == NULL)
		{
			break;
		}
		va_start(ap, cstring_format);
		o = cco_vString_newWithVformat(cstring_format, ap);
		va_end(ap);
		cco_vString_catenate(base_o, o);
		cco_release(o);
	} while (0);
	return;
}

static char *cco_vString_format_sub_itoa(char *buf, int buflen, int i)
{
	char *pos = buf + buflen - 1;
	unsigned int u;
	int negative = 0;

	if (i < 0) {
		negative = 1;
		u = ((unsigned int)(-(1+i))) + 1;
	} else {
		u = i;
	}

	*pos = 0;
	do {
		*--pos = '0' + (u % 10);
		u /= 10;
	} while (u);

	if (negative) {
		*--pos = '-';
	}
	return pos;
}

cco_vString *cco_vString_getReplasedString(cco_vString *string, char *target_regex,
		cco_vString *replase_string)
{
	regex_t preg;
	regmatch_t pmatch[1];
	cco_vString *work_string;
	char *build_string;
	int build_string_length;
	char *check_string;
	int check_string_length;
	cco_vString *result = NULL;

	do {
		if (regcomp(&preg, target_regex, REG_EXTENDED) != 0)
		{
			break;
		}
		build_string = cco_vString_getCstring(string);
		build_string_length = cco_vString_length(string);

		result = cco_vString_new("");
		/* do replace */
		check_string = build_string;
		check_string_length = build_string_length;
		while (regexec(&preg, check_string, 1, pmatch, 0) == 0)
		{
			/* fetch string to match string before. */
			check_string[pmatch[0].rm_so] = 0;
			work_string = cco_vString_new(check_string);
			cco_vString_catenate(result, work_string);
			cco_release(work_string);
			/* catenates replase_string. */
			/*
			 * TODO: checks inheritanceId
			 */
			cco_vString_catenate(result, replase_string);
			/* changes a status of string. */
			check_string = check_string + pmatch[0].rm_eo;
			check_string_length -= pmatch[0].rm_eo;
			//printf("%d - %d(%d)\n", pmatch[0].rm_so, pmatch[0].rm_eo, check_string_length);
		}
		/* catenates last string. */
		work_string = cco_vString_new(check_string);
		cco_vString_catenate(result, work_string);
		cco_release(work_string);

		free(build_string);
		regfree(&preg);
	} while(0);
	return result;
}

cco_vString *cco_vString_getReplasedStringWithCstring(cco_vString *string, char *target_regex,
		char *replase_cstring)
{
	cco_vString *replase_string;
	cco_vString *result = NULL;

	replase_string = cco_vString_new(replase_cstring);
	result = cco_vString_getReplasedString(string, target_regex, replase_string);
	cc_string_release(replase_string);
	return result;
}

int cco_vString_toInt(cco_vString *string)
{
	char *cstring;
	int result = -1;

	if (string != NULL)
	{
		cstring = cco_vString_getCstring(string);
		result = atoi(cstring);
		free(cstring);
	}
	return result;
}
