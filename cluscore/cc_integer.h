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
 * The Integer Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_INTEGER_H
#define CC_INTEGER_H

#include "cc_object.h"

extern int g_cc_integer_id;
typedef cc_object cc_integer;

cc_integer *cc_integer_new(int value);
void cc_integer_release(cc_integer *object);
int cc_integer_get(cc_integer *object, int *out_value);
cc_integer *cc_integer_set(cc_integer *object, int value);
cc_integer *cc_integer_plus(cc_integer *plused_object, cc_integer *object);
cc_integer *cc_integer_minus(cc_integer *minused_object, cc_integer *object);
char *cc_integer_tocstring(cc_integer *object);
int cc_integer_compere(cc_integer *object1, cc_integer *object2);

#endif /*CC_INTEGER_H*/
