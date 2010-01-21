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

#include "cco_vXml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	o->vXml_data = cco_vStringnew("");
	o->vXml_attributes = cco_redblacktree_new();
	o->vXml_elements = cco_redblacktree_new();
	o->vXml_elementsList = cco_arraylist_new();

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
	cco_vXml_properties *properties;
	do
	{
		if (xml == NULL) {
			break;
		}
		properties = xml->properties;
		cco_release(properties->name);
		properties->name = cco_vStringnew(str);
	} while (0);
	return;
}

void cco_vXml_setData(cco_vXml *xml, char *str)
{
	cco_vXml_properties *properties;
	do
	{
		if (xml == NULL) {
			break;
		}
		properties = xml->properties;
		cco_release(properties->data);
		properties->data = cco_vString_new(str);
	} while (0);
	return;
}

void cco_vXml_addChildElement(cco_vXml *element, cco_vXml *child_element)
{
	cco_vXml_properties *properties;
	cco_vXml_properties *child_properties;
	cco_arraylist *target_elements;

	if (element != NULL && child_element != NULL)
	{
		properties = element->properties;
		child_properties = child_element->properties;
		target_elements = cco_redblacktree_get(properties->elements, child_properties->name);
		if (target_elements == NULL)
		{
			target_elements = cco_arraylist_new();
			cco_redblacktree_insert(properties->elements, child_properties->name, target_elements);
		}
		cco_arraylist_addAtBack(target_elements, child_element);
		cco_release(target_elements);
		cco_arraylist_addAtBack(properties->elementsList, child_element);
	}
	return;
}

void cco_vXml_addAttribute(cco_vXml *element, cco_vString *attrinute_name, cco_vString *attrinute_value)
{
	cco_vXml_properties *properties;

	if (element != NULL && attrinute_name != NULL && attrinute_value != NULL)
	{
		properties = element->properties;
		cco_redblacktree_insertWithReplace(properties->attributes, attrinute_name, attrinute_value);
	}
	return;
}

#if 0
/* use xmlTextReader. */
void cco_vXml_read_parse_node(cco_vXml *xml, xmlTextReaderPtr reader)
{
	xmlElementType node_type;

	cco_vXml_properties *properties;
	cco_redblacktree *element_tree;

	cco_vXml *new_element;
	cco_vString *element_name;
	xmlChar *element_name_str;
	cco_arraylist *target_elements;
	int attribute_index;
	int attribute_count;
	xmlChar *attribute_value;

	while (xmlTextReaderRead(reader) == 1) {
		node_type = xmlTextReaderNodeType(reader);
		if (node_type == XML_READER_TYPE_ELEMENT) {
			/* Allocates memory for work. */
			element_name_str = xmlTextReaderName(reader);
			new_element = cco_vXml_new();
			/* Adds child element to this element. */
			cco_vXml_setName(new_element, element_name_str);
			attribute_count = xmlTextReaderAttributeCount(reader);
			for (attribute_index = 0; attribute_index < attribute_count; attribute_index++)
			{
				attribute_value = xmlTextReaderGetAttributeNo(reader, attribute_index);
				xmlFree(attribute_value);
			}
			if (xmlTextReaderIsEmptyElement(reader) == 0) {
				cco_vXml_read_parse_node(new_element, reader);
			}
			cco_vXml_addChildElement(xml, new_element);
			/* Releases memory for work. */
			cco_release(new_element);
			xmlFree(element_name_str);
		} else if (node_type == XML_READER_TYPE_END_ELEMENT) {
			break;
		} else if (node_type == XML_READER_TYPE_TEXT) {
			element_name_str = xmlTextReaderValue(reader);
			/* Sets data to this element. */
			cco_vXml_setData(xml, element_name_str);
			xmlFree(element_name_str);
		} else if (node_type == XML_READER_TYPE_CDATA) {
			element_name_str = xmlTextReaderValue(reader);
			/* Sets data to this element. */
			cco_vXml_setData(xml, element_name_str);
			xmlFree(element_name_str);
		}
	}
	return;
}

void cco_vXml_read(cco_vXml *xml, char *uri)
{
	xmlTextReaderPtr reader;

	reader = xmlNewTextReaderFilename(uri);
	cco_vXml_read_parse_node(xml, reader);
	xmlFreeTextReader(reader);
	xmlCleanupParser();
	return;
}
#else
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
#endif

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
	cco_vXml_properties *properties;
	cco_arraylist *path_list;
	cco_redblacktree *element_tree;
	cco_vString *path_str;
	cco_arraylist *element_list = NULL;
	cco_vXml *cur_xml;
	cco_vXml_properties *cur_xml_properties;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		properties = xml->properties;
		path_list = cco_vXml_getElements_getParsePath(xml, path);
		element_tree = properties->elements;
		cco_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = cco_arraylist_getAtCursor(path_list);
			element_list = cco_redblacktree_get(element_tree, path_str);
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
			cur_xml = cco_arraylist_getAtCursor(element_list);
			cur_xml_properties = cur_xml->properties;
			element_tree = cur_xml_properties->elements;
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
		get_xml = cco_arraylist_getAtFront(get_xml_list);
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
		get_xml = cco_arraylist_getAtBack(get_xml_list);
		cco_release(get_xml_list);
	} while (0);

	return get_xml;
}

cco_arraylist *cco_vXml_getChildElements(cco_vXml *xml, char *path)
{
	cco_vXml_properties *properties;
	cco_arraylist *path_list;
	cco_redblacktree *element_tree;
	cco_vString *path_str;
	cco_arraylist *element_list = NULL;
	cco_arraylist *targetelement_list = NULL;
	cco_vXml *cur_xml;
	cco_vXml_properties *cur_xml_properties = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		cur_xml_properties = xml->properties;
		path_list = cco_vXml_getElements_getParsePath(xml, path);
		element_tree = cur_xml_properties->elements;
		cco_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = cco_arraylist_getAtCursor(path_list);
			element_list = cco_redblacktree_get(element_tree, path_str);
			cco_release(path_str);
			if (element_list == NULL)
			{
				break;
			}
			cco_arraylist_setCursorAtFront(element_list);
			cur_xml = cco_arraylist_getAtCursor(element_list);
			cur_xml_properties = cur_xml->properties;
			targetelement_list = cur_xml_properties->elementsList;
			cco_grab(cur_xml_properties->elementsList);
			element_tree = cur_xml_properties->elements;
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
	cco_vXml_properties *properties;
	if (xml != NULL)
	{
		properties = xml->properties;
		cco_grab(properties->name);
		result_str = properties->name;
	}
	return result_str;
}

cco_vString *cco_vXml_getContent(cco_vXml *xml)
{
	cco_vString *result_str = NULL;
	cco_vXml_properties *properties;
	if (xml != NULL)
	{
		properties = xml->properties;
		cco_grab(properties->data);
		result_str = properties->data;
	}
	return result_str;
}

cco_vString *cco_vXml_getAttribute(cco_vXml *xml, char* name)
{
	cco_vString *result_str = NULL;
	cco_vXml_properties *properties;
	cco_vString *name_str;
	if (xml != NULL)
	{
		properties = xml->properties;
		name_str = cco_vString_new(name);
		result_str = cco_redblacktree_get(properties->attributes, name_str);
		cco_release(name_str);
	}
	return result_str;
}


void cco_vXml_rbtree_attribute_sub(cco *callbackobject, cco *key, cco *object)
{
	char *key_cstr;
	char *obj_cstr;
	cco_vString *key_str;
	cco_vString *obj_str;
	cco_vString *str1;
	cco_vString *str2;
	cco_vString *str3;

	str1 = cco_vString_new(" ");
	str2 = cco_vString_new("=\"");
	str3 = cco_vString_new("\"");

	key_cstr = key->tocstring(key);
	obj_cstr = object->tocstring(object);
	key_str = cco_vString_new(key_cstr);
	obj_str = cco_vString_new(obj_cstr);
	cco_vString_catenate(callbackobject, str1);
	cco_vString_catenate(callbackobject, key_str);
	cco_vString_catenate(callbackobject, str2);
	cco_vString_catenate(callbackobject, obj_str);
	cco_vString_catenate(callbackobject, str3);
	cco_release(key_str);
	cco_release(obj_str);
	free(key_cstr);
	free(obj_cstr);

	cco_release(str1);
	cco_release(str2);
	cco_release(str3);

	return;
}

void cco_vXml_rbtree_element_sub(cco *callbackobject, cco *key, cco *object)
{
	char *obj_cstr;
	cco_vString *obj_str;
	cco_vXml *xml;

	cco_arraylist_setCursorAtFront(object);
	while(xml = cco_arraylist_getAtCursor(object))
	{
		obj_cstr = xml->tocstring(xml);
		obj_str = cco_vString_new(obj_cstr);
		cco_vString_catenate(callbackobject, obj_str);
		cco_release(obj_str);
		cco_release(xml);
		cco_arraylist_setCursorAtNext(object);
	}
	return;
}

char *cco_vXml_tocstring(cco_vXml *xml)
{
	cco_vXml_properties *properties;
	cco_vString *result_string;
	cco_vString *string;
	char *cstring;

	properties = xml->properties;

	/* element name */
	result_string = cco_vString_new("");
	if (cco_vString_length(properties->name) != 0)
	{
		string = cco_vString_new("<");
		cco_vString_catenate(result_string, string);
		cco_release(string);
		cco_vString_catenate(result_string, properties->name);

		/* attribute */
		cco_redblacktree_traversePreorder(properties->attributes, result_string, cco_vXml_rbtree_attribute_sub);

		string = cco_vString_new(">");
		cco_vString_catenate(result_string, string);
		cco_release(string);

		string = cco_vString_new("<![CDATA[");
		cco_vString_catenate(result_string, string);
		cco_release(string);

		/* content */
		cco_vString_catenate(result_string, properties->data);

		string = cco_vString_new("]]>");
		cco_vString_catenate(result_string, string);
		cco_release(string);
	}
	/* child elements */
	cco_redblacktree_traversePreorder(properties->elements, result_string, cco_vXml_rbtree_element_sub);

	if (cco_vString_length(properties->name) != 0)
	{
		string = cco_vString_new("</");
		cco_vString_catenate(result_string, string);
		cco_release(string);
		cco_vString_catenate(result_string, properties->name);
		string = cco_vString_new(">\n");
		cco_vString_catenate(result_string, string);
		cco_release(string);
	}

	cstring = result_string->tocstring(result_string);
	cco_release(result_string);
	return cstring;
}

