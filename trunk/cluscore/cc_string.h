/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
 *  Copyright (c) 2009 ClusCore
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
 * The String Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_STRING_H
#define CC_STRING_H

#include "cc_object.h"
#include "cc_arraylist.h"

extern int g_cc_string_object_id;
typedef cc_object cc_string;

cc_string *cc_string_new(char *string);
cc_string *cc_string_newWithLength(char *string, int length);
void cc_string_release(cc_string *string);
char *cc_string_tocstring(cc_string *string);
int cc_string_compare(cc_string *string1, cc_string *string2);
int cc_string_get_hashcode(cc_string *string, int salt);
int cc_string_length(cc_string *string);
void cc_string_catenate(cc_string *base_string, cc_object *string);
void cc_string_replase(cc_string *string, char *target_regex,
		cc_object *replase_string);
void cc_string_replase_cstring(cc_string *string, char *target_regex,
		char *replase_cstring);
void cc_string_renew(cc_string *string, char *cstring);

#endif
