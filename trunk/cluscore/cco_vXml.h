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
 * The cco_vXml Class for ClusCore.
 *
 * Author:
 */

#ifndef CCO_VXML_H_
#define CCO_VXML_H_

#include "cco_v.h"
#include "cco_vString.h"
#include "cco_arraylist.h"
#include "cco_redblacktree.h"

#define CCO_VXML_PROPERTIES \
	cco_vString *vXml_name;\
	cco_vString *vXml_data;\
	cco_redblacktree *vXml_attributes;\
	cco_redblacktree *vXml_elements;\
	cco_arraylist *vXml_elementsList;

typedef struct cco_vXml cco_vXml;

struct cco_vXml {
	CCO_PROPERTIES
	CCO_V_PROPERTIES
	CCO_VXML_PROPERTIES
};

cco_vXml *cco_vXml_baseNew(int size);
void cco_vXml_baseRelease(void *cco);
void cco_vXml_baseInitialize(cco_vXml *cco);
void cco_vXml_baseFinalize(cco_vXml *cco);
cco_vXml *cco_vXml_new();
void cco_vXml_release(void *cco);

void cco_vXml_read(cco_vXml *xml, char *uri);
void cco_vXml_readBuffer(cco_vXml *xml, char *buff);
cco_arraylist *cco_vXml_getElements(cco_vXml *xml, char *path);
cco_vXml *cco_vXml_getElementAtFront(cco_vXml *xml, char *path);
cco_vXml *cco_vXml_getElementAtBack(cco_vXml *xml, char *path);
cco_arraylist *cco_vXml_getChildElements(cco_vXml *xml, char *path);
cco_vString *cco_vXml_getName(cco_vXml *xml);
cco_vString *cco_vXml_getContent(cco_vXml *xml);
cco_vString *cco_vXml_getAttribute(cco_vXml *xml, char* name);

/* Don't touch following comment.
CCOINHERITANCE:CCO_PROPERTIES
CCOINHERITANCE:CCO_V_PROPERTIES
CCOINHERITANCE:CCO_VXML_PROPERTIES
*/

#endif /* CCO_VXML_H_ */
