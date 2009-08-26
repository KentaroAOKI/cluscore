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
 * The Stopwatch Class for ClusCore.
 * http://www.cluscore.com/
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_STOPWATCH_H
#define CC_STOPWATCH_H

#include "cc_object.h"

extern int g_cc_stopwatch_object_id;
typedef cc_object cc_stopwatch;

cc_stopwatch *cc_stopwatch_new(void);
void cc_stopwatch_release(cc_stopwatch *stopwatch);
void cc_stopwatch_start(cc_stopwatch *stopwatch);
void cc_stopwatch_stop(cc_stopwatch *stopwatch);
void cc_stopwatch_reset(cc_stopwatch *stopwatch);
char *cc_stopwatch_tocstring(cc_stopwatch *stopwatch);
int cc_stopwatch_compare(cc_stopwatch *stopwatch1, cc_stopwatch *stopwatch2);

#endif
