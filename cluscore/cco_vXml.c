/*
 *  Copyright (c) 2008-2009 Kentaro Aoki
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
 * Author: kentaro.aoki at gmail.com
 */

#include "cco_vXml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>

char *cco_vXml_getCstring(void *obj);

cco_defineClass(cco_vXml);

cco_vXml *cco_vXml_baseNew(int size)
{
	cco_vXml *o = NULL;
	do {
		if (size < sizeof(cco_vXml))
		{
			break;
		}
		o = (cco_vXml *)cco_v_baseNew(size);
		if (o == NULL)
		{
			break;
		}
		cco_setClass(o, cco_vXml);
		cco_vXml_baseInitialize(o);
	} while (0);
	return o;
}

void cco_vXml_baseRelease(void *o)
{
	cco_vXml_baseFinalize(o);
	cco_v_baseRelease(o);
}

void cco_vXml_baseInitialize(cco_vXml *o)
{
	o->baseRelease = &cco_vXml_baseRelease;

	o->vXml_name = cco_vString_new("");
	o->vXml_data = cco_vString_new("");
	o->vXml_attributes = cco_redblacktree_new();
	o->vXml_elements = cco_redblacktree_new();
	o->vXml_elementsList = cco_arraylist_new();
	o->v_getCstring = &cco_vXml_getCstring;

	return;
}

void cco_vXml_baseFinalize(cco_vXml *o)
{
	cco_release(o->vXml_name);
	cco_release(o->vXml_data);
	cco_release(o->vXml_attributes);
	cco_release(o->vXml_elements);
	cco_release(o->vXml_elementsList);
	return;
}

cco_vXml *cco_vXml_new()
{
	return cco_vXml_baseNew(sizeof(cco_vXml));
}

void cco_vXml_release(void *o)
{
	cco_release(o);
}

void cco_vXml_setName(cco_vXml *xml, char *str)
{
	do
	{
		if (xml == NULL) {
			break;
		}
		cco_release(xml->vXml_name);
		xml->vXml_name = cco_vString_new(str);
	} while (0);
	return;
}

void cco_vXml_setData(cco_vXml *xml, char *str)
{
	do
	{
		if (xml == NULL) {
			break;
		}
		cco_release(xml->vXml_data);
		xml->vXml_data = cco_vString_new(str);
	} while (0);
	return;
}

void cco_vXml_addChildElement(cco_vXml *element, cco_vXml *child_element)
{
	cco_arraylist *target_elements;

	if (element != NULL && child_element != NULL)
	{
		target_elements = (cco_arraylist *)cco_redblacktree_get(element->vXml_elements, (cco_v *)child_element->vXml_name);
		if (target_elements == NULL)
		{
			target_elements = cco_arraylist_new();
			cco_redblacktree_insert(element->vXml_elements, (cco_v *)child_element->vXml_name, (cco *)target_elements);
		}
		cco_arraylist_addAtBack(target_elements, child_element);
		cco_release(target_elements);
		cco_arraylist_addAtBack(element->vXml_elementsList, child_element);
	}
	return;
}

void cco_vXml_addAttribute(cco_vXml *element, cco_vString *attrinute_name, cco_vString *attrinute_value)
{
	if (element != NULL && attrinute_name != NULL && attrinute_value != NULL)
	{
		cco_redblacktree_insertWithReplace(element->vXml_attributes, (cco_v *)attrinute_name, (cco *)attrinute_value);
	}
	return;
}

/* use xmlTextReader. */
void cco_vXml_read_parse_node(cco_vXml *xml, xmlNodePtr this_node)
{
	cco_vXml *new_element;
	cco_vString *element_name;
	xmlChar *element_name_str;

    xmlNode *cur_node = NULL;
    xmlAttr *cur_attr = NULL;
    xmlChar *content_cstr;
	cco_vString *name_str;
	cco_vString *content_str;

	for (cur_node = this_node; cur_node; cur_node = cur_node->next) {
		if (xmlIsBlankNode(cur_node) != 0)
		{
			/* If this node is blank, go to top of loop. */
			continue;
		}
        if (cur_node->type == XML_ELEMENT_NODE) {
			/* Allocates memory for an element. */
			new_element = cco_vXml_new();
			/* Adds child element to this element. */
			cco_vXml_setName(new_element, (char *)cur_node->name);
			/* Adds some attributes to this element. */
            for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
    			/* Allocates memory for an attribute. */
            	content_cstr = xmlGetProp(cur_node, cur_attr->name);
            	name_str = cco_vString_new((char *)cur_attr->name);
            	content_str = cco_vString_new(content_cstr);
            	/* Inserts an attribute to this element. */
            	cco_vXml_addAttribute(new_element, name_str, content_str);
    			/* Releases memory for an attribute. */
            	xmlFree(content_cstr);
            	cco_release(name_str);
            	cco_release(content_str);
            }
			cco_vXml_read_parse_node(new_element, cur_node->children);
			cco_vXml_addChildElement(xml, new_element);
			/* Releases memory for an element. */
			cco_release(new_element);
        } else if (cur_node->type == XML_TEXT_NODE || cur_node->type == XML_CDATA_SECTION_NODE) {
        	cco_vXml_setData(xml, cur_node->content);
        }
    }
	return;
}

void cco_vXml_read(cco_vXml *xml, char *uri)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	do {
		if (xml == NULL || uri == NULL)
		{
			break;
		}
		doc = xmlReadFile(uri, NULL, 0);
		if (doc == NULL)
		{
			break;
		}
		cur = xmlDocGetRootElement(doc);
		cco_vXml_read_parse_node(xml, cur);
	} while(0);
	if (doc != NULL) {
		xmlFreeDoc(doc);
	}
	return;
}
void cco_vXml_readBuffer(cco_vXml *xml, char *buff)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	do {
		if (xml == NULL || buff == NULL)
		{
			break;
		}
		doc = xmlReadMemory(buff, strlen(buff), NULL, NULL, 0);
		if (doc == NULL)
		{
			break;
		}
		cur = xmlDocGetRootElement(doc);
		cco_vXml_read_parse_node(xml, cur);
	} while(0);
	if (doc != NULL) {
		xmlFreeDoc(doc);
	}
	return;
}

cco_arraylist *cco_vXml_getElements_getParsePath(cco_vXml *xml, char *path)
{
	char *path_from;
	char *path_to;
	char *path_dup;
	cco_arraylist *path_list = NULL;
	cco_vString *path_str;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		path_list = cco_arraylist_new();
		path_dup = strdup(path);

		path_from = path_dup;
		while(1)
		{
			path_to = strchr(path_from, '/');
			if (path_to == NULL)
			{
				path_str = cco_vString_new(path_from);
				cco_arraylist_addAtBack(path_list, path_str);
				cco_release(path_str);
				break;
			}
			*path_to = 0;
			path_to ++;
			path_str = cco_vString_new(path_from);
			cco_arraylist_addAtBack(path_list, path_str);
			cco_release(path_str);
			path_from = path_to;
		}
		free(path_dup);
	} while (0);
	return path_list;
}

cco_arraylist *cco_vXml_getElements(cco_vXml *xml, char *path)
{
	cco_arraylist *path_list;
	cco_redblacktree *element_tree;
	cco_vString *path_str;
	cco_arraylist *element_list = NULL;
	cco_vXml *cur_xml;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		path_list = cco_vXml_getElements_getParsePath(xml, path);
		element_tree = xml->vXml_elements;
		cco_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = (cco_vString *)cco_arraylist_getAtCursor(path_list);
			element_list = (cco_arraylist *)cco_redblacktree_get(element_tree, (cco_v *)path_str);
			cco_release(path_str);
			if (element_list == NULL)
			{
				break;
			}
			cco_arraylist_setCursorAtFront(element_list);
			if (cco_arraylist_setCursorAtNext(path_list) < 0)
			{
				break;
			}
			cur_xml = (cco_vXml *)cco_arraylist_getAtCursor(element_list);
			element_tree = cur_xml->vXml_elements;
			cco_release(cur_xml);
			cco_release(element_list);
		}
		cco_release(path_list);
	} while (0);
	return element_list;
}

cco_vXml *cco_vXml_getElementAtFront(cco_vXml *xml, char *path)
{
	cco_arraylist *get_xml_list;
	cco_vXml *get_xml = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		get_xml_list = cco_vXml_getElements(xml, path);
		if (get_xml_list == NULL) {
			break;
		}
		get_xml = (cco_vXml *)cco_arraylist_getAtFront(get_xml_list);
		cco_release(get_xml_list);
	} while (0);

	return get_xml;
}

cco_vXml *cco_vXml_getElementAtBack(cco_vXml *xml, char *path)
{
	cco_arraylist *get_xml_list;
	cco_vXml *get_xml = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		get_xml_list = cco_vXml_getElements(xml, path);
		if (get_xml_list == NULL) {
			break;
		}
		get_xml = (cco_vXml *)cco_arraylist_getAtBack(get_xml_list);
		cco_release(get_xml_list);
	} while (0);

	return get_xml;
}

cco_arraylist *cco_vXml_getChildElements(cco_vXml *xml, char *path)
{
	cco_arraylist *path_list;
	cco_redblacktree *element_tree;
	cco_vString *path_str;
	cco_arraylist *element_list = NULL;
	cco_arraylist *targetelement_list = NULL;
	cco_vXml *cur_xml;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		path_list = cco_vXml_getElements_getParsePath(xml, path);
		element_tree = xml->vXml_elements;
		cco_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = (cco_vString *)cco_arraylist_getAtCursor(path_list);
			element_list = (cco_arraylist *)cco_redblacktree_get(element_tree, (cco_v*)path_str);
			cco_release(path_str);
			if (element_list == NULL)
			{
				break;
			}
			cco_arraylist_setCursorAtFront(element_list);
			cur_xml = (cco_vXml *)cco_arraylist_getAtCursor(element_list);
			targetelement_list = cur_xml->vXml_elementsList;
			cco_grab(cur_xml->vXml_elementsList);
			element_tree = cur_xml->vXml_elements;
			cco_release(cur_xml);
			cco_release(element_list);
			if (cco_arraylist_setCursorAtNext(path_list) < 0)
			{
				break;
			}
		}
		cco_release(path_list);
	} while (0);
	return targetelement_list;
}

cco_vString *cco_vXml_getName(cco_vXml *xml)
{
	cco_vString *result_str = NULL;
	if (xml != NULL)
	{
		cco_grab(xml->vXml_name);
		result_str = xml->vXml_name;
	}
	return result_str;
}

cco_vString *cco_vXml_getContent(cco_vXml *xml)
{
	cco_vString *result_str = NULL;
	if (xml != NULL)
	{
		cco_grab(xml->vXml_data);
		result_str = xml->vXml_data;
	}
	return result_str;
}

cco_vString *cco_vXml_getAttribute(cco_vXml *xml, char* name)
{
	cco_vString *result_str = NULL;
	cco_vString *name_str;
	if (xml != NULL)
	{
		name_str = cco_vString_new(name);
		result_str = (cco_vString *)cco_redblacktree_get(xml->vXml_attributes, (cco_v *)name_str);
		cco_release(name_str);
	}
	return result_str;
}

void cco_vXml_rbtree_attribute_sub(cco *callbackobject, cco_v *key, cco *object)
{
	char *key_cstr;
	char *obj_cstr;
	cco_vString *key_str;
	cco_vString *obj_str;
	cco_vString *str1;
	cco_vString *str2;
	cco_vString *str3;

	cco_v *object_v = (cco_v *)object;
	cco_vString *callbackobject_string = (cco_vString *)callbackobject;

	str1 = cco_vString_new(" ");
	str2 = cco_vString_new("=\"");
	str3 = cco_vString_new("\"");

	key_cstr = key->v_getCstring(key);
	obj_cstr = object_v->v_getCstring(object_v);
	key_str = cco_vString_new(key_cstr);
	obj_str = cco_vString_new(obj_cstr);
	cco_vString_catenate(callbackobject_string, str1);
	cco_vString_catenate(callbackobject_string, key_str);
	cco_vString_catenate(callbackobject_string, str2);
	cco_vString_catenate(callbackobject_string, obj_str);
	cco_vString_catenate(callbackobject_string, str3);
	cco_release(key_str);
	cco_release(obj_str);
	free(key_cstr);
	free(obj_cstr);

	cco_release(str1);
	cco_release(str2);
	cco_release(str3);

	return;
}

void cco_vXml_rbtree_element_sub(cco *callbackobject, cco_v *key, cco *object)
{
	char *obj_cstr;
	cco_vString *obj_str;
	cco_vXml *xml;
	cco_arraylist *list_object = (cco_arraylist *)object;

	cco_arraylist_setCursorAtFront(list_object);
	while(xml = (cco_vXml *)cco_arraylist_getAtCursor(list_object))
	{
		obj_cstr = xml->v_getCstring(xml);
		obj_str = cco_vString_new(obj_cstr);
		cco_vString_catenate((cco_vString *)callbackobject, obj_str);
		cco_release(obj_str);
		cco_release(xml);
		cco_arraylist_setCursorAtNext(list_object);
	}
	return;
}

char *cco_vXml_getCstring(void *obj)
{
	cco_vXml *xml = (cco_vXml *)obj;
	cco_vString *result_string;
	cco_vString *string;
	char *cstring;

	/* element name */
	result_string = cco_vString_new("");
	if (cco_vString_length(xml->vXml_name) != 0)
	{
		string = cco_vString_newWithFormat("<%@", xml->vXml_name);
		cco_vString_catenate(result_string, string);
		cco_release(string);

		/* attribute */
		cco_redblacktree_traversePreorder(xml->vXml_attributes, (cco *)result_string, cco_vXml_rbtree_attribute_sub);

		string = cco_vString_new(">");
		cco_vString_catenate(result_string, string);
		cco_release(string);

		/* content */
		string = cco_vString_newWithFormat("<![CDATA[%@]]>", xml->vXml_data);
		cco_vString_catenate(result_string, string);
		cco_release(string);
	}
	/* child elements */
	cco_redblacktree_traversePreorder(xml->vXml_elements, (cco *)result_string, cco_vXml_rbtree_element_sub);

	if (cco_vString_length(xml->vXml_name) != 0)
	{
		string = cco_vString_newWithFormat("</%@>\n", xml->vXml_name);
		cco_vString_catenate(result_string, string);
		cco_release(string);
	}

	cstring = result_string->v_getCstring(result_string);
	cco_release(result_string);
	return cstring;
}

