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
 * THe XML Class for ClusCore.
 *
 * Author: kentaro.aoki@gmail.com
 */

#ifndef CC_XML_H_
#define CC_XML_H_

#include "cc_object.h"
#include "cc_arraylist.h"
#include "cc_hashtable.h"
#include "cc_redblacktree.h"
#include "cc_string.h"

extern int g_cc_xml_object_id;
typedef cc_object cc_xml;
typedef struct cc_xml_properties cc_xml_properties;

struct cc_xml_properties
{
	cc_string *name;
	cc_string *data;
	cc_redblacktree *attributes;
	cc_redblacktree *elements;
};

cc_xml *cc_xml_new(void);
void cc_xml_release(cc_xml *list);
char *cc_xml_tocstring(cc_xml *list);
void cc_xml_read(cc_xml *xml, char *uri);
void cc_xml_readBuffer(cc_xml *xml, char *buff);
cc_arraylist *cc_xml_getElements(cc_xml *xml, char *path);
cc_xml *cc_xml_getElementAtFront(cc_xml *xml, char *path);
cc_xml *cc_xml_getElementAtBack(cc_xml *xml, char *path);
cc_string *cc_xml_getName(cc_xml *xml);
cc_string *cc_xml_getContent(cc_xml *xml);
cc_string *cc_xml_getAttribute(cc_xml *xml, char* name);


#endif /* CC_XML_H_ */
