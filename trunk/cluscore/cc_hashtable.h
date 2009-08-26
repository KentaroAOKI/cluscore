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
 * The Hash Table Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_HASHTABLE_H
#define CC_HASHTABLE_H

#include "cc_object.h"

extern int g_cc_hashtable_object_id;
typedef cc_object cc_hashtable;

cc_hashtable *cc_hashtable_new(int size);
void cc_hashtable_release(cc_hashtable *table);
void cc_hashtable_add(cc_hashtable *table, cc_object *key, cc_object *object);
void cc_hashtable_set(cc_hashtable *table, cc_object *key, cc_object *object);
int cc_hashtable_contain(cc_hashtable *table, cc_object *key);
cc_object *cc_hashtable_get(cc_hashtable *table, cc_object *key);
int cc_hashtable_collision(cc_hashtable *table);
char *cc_hashtable_tocstring(cc_hashtable *table);

#endif
