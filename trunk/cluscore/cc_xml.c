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
 * The XML Class for ClusCore.
 *
 * Author: kentaro.aoki@gmail.com
 */

#include <stdlib.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include "cc_xml.h"

int g_cc_xml_object_id;

cc_xml_properties *cc_xml_properties_new(void)
{
	cc_xml_properties *properties;
	properties = malloc(sizeof(cc_xml_properties));
	if (properties != NULL)
	{
		properties->name = cc_string_new("");
		properties->data = cc_string_new("");
		properties->attributes = cc_redblacktree_new();
		properties->elements = cc_redblacktree_new();
		properties->elementsList = cc_arraylist_new();
	}
	return properties;
}

void cc_xml_properties_dispose(cc_xml_properties *properties)
{
	cc_object_release(properties->name);
	cc_object_release(properties->data);
	cc_object_release(properties->attributes);
	cc_object_release(properties->elements);
	cc_object_release(properties->elementsList);
	free(properties);
	return;
}

cc_xml *cc_xml_new(void)
{
	cc_xml *object = NULL;
	cc_xml_properties *properties = cc_xml_properties_new();
	object = cc_object_new(&g_cc_xml_object_id, properties, cc_xml_properties_dispose);
	object->tocstring = (void *) cc_xml_tocstring;
	return (object);
}

void cc_xml_release(cc_xml *xml)
{
	cc_object_release(xml);
	return;
}

void cc_xml_setName(cc_xml *xml, char *str)
{
	cc_xml_properties *properties;
	do
	{
		if (xml == NULL) {
			break;
		}
		properties = xml->properties;
		cc_object_release(properties->name);
		properties->name = cc_string_new(str);
	} while (0);
	return;
}

void cc_xml_setData(cc_xml *xml, char *str)
{
	cc_xml_properties *properties;
	do
	{
		if (xml == NULL) {
			break;
		}
		properties = xml->properties;
		cc_object_release(properties->data);
		properties->data = cc_string_new(str);
	} while (0);
	return;
}

void cc_xml_addChildElement(cc_xml *element, cc_xml *child_element)
{
	cc_xml_properties *properties;
	cc_xml_properties *child_properties;
	cc_arraylist *target_elements;

	if (element != NULL && child_element != NULL)
	{
		properties = element->properties;
		child_properties = child_element->properties;
		target_elements = cc_redblacktree_get(properties->elements, child_properties->name);
		if (target_elements == NULL)
		{
			target_elements = cc_arraylist_new();
			cc_redblacktree_insert(properties->elements, child_properties->name, target_elements);
		}
		cc_arraylist_addAtBack(target_elements, child_element);
		cc_object_release(target_elements);
		cc_arraylist_addAtBack(properties->elementsList, child_element);
	}
	return;
}

void cc_xml_addAttribute(cc_xml *element, cc_string *attrinute_name, cc_string *attrinute_value)
{
	cc_xml_properties *properties;

	if (element != NULL && attrinute_name != NULL && attrinute_value != NULL)
	{
		properties = element->properties;
		cc_redblacktree_insertWithReplace(properties->attributes, attrinute_name, attrinute_value);
	}
	return;
}

#if 0
/* use xmlTextReader. */
void cc_xml_read_parse_node(cc_xml *xml, xmlTextReaderPtr reader)
{
	xmlElementType node_type;

	cc_xml_properties *properties;
	cc_redblacktree *element_tree;

	cc_xml *new_element;
	cc_string *element_name;
	xmlChar *element_name_str;
	cc_arraylist *target_elements;
	int attribute_index;
	int attribute_count;
	xmlChar *attribute_value;

	while (xmlTextReaderRead(reader) == 1) {
		node_type = xmlTextReaderNodeType(reader);
		if (node_type == XML_READER_TYPE_ELEMENT) {
			/* Allocates memory for work. */
			element_name_str = xmlTextReaderName(reader);
			new_element = cc_xml_new();
			/* Adds child element to this element. */
			cc_xml_setName(new_element, element_name_str);
			attribute_count = xmlTextReaderAttributeCount(reader);
			for (attribute_index = 0; attribute_index < attribute_count; attribute_index++)
			{
				attribute_value = xmlTextReaderGetAttributeNo(reader, attribute_index);
				xmlFree(attribute_value);
			}
			if (xmlTextReaderIsEmptyElement(reader) == 0) {
				cc_xml_read_parse_node(new_element, reader);
			}
			cc_xml_addChildElement(xml, new_element);
			/* Releases memory for work. */
			cc_object_release(new_element);
			xmlFree(element_name_str);
		} else if (node_type == XML_READER_TYPE_END_ELEMENT) {
			break;
		} else if (node_type == XML_READER_TYPE_TEXT) {
			element_name_str = xmlTextReaderValue(reader);
			/* Sets data to this element. */
			cc_xml_setData(xml, element_name_str);
			xmlFree(element_name_str);
		} else if (node_type == XML_READER_TYPE_CDATA) {
			element_name_str = xmlTextReaderValue(reader);
			/* Sets data to this element. */
			cc_xml_setData(xml, element_name_str);
			xmlFree(element_name_str);
		}
	}
	return;
}

void cc_xml_read(cc_xml *xml, char *uri)
{
	xmlTextReaderPtr reader;

	reader = xmlNewTextReaderFilename(uri);
	cc_xml_read_parse_node(xml, reader);
	xmlFreeTextReader(reader);
	xmlCleanupParser();
	return;
}
#else
/* use xmlTextReader. */
void cc_xml_read_parse_node(cc_xml *xml, xmlNodePtr this_node)
{
	cc_xml *new_element;
	cc_string *element_name;
	xmlChar *element_name_str;

    xmlNode *cur_node = NULL;
    xmlAttr *cur_attr = NULL;
    xmlChar *content_cstr;
	cc_string *name_str;
	cc_string *content_str;

	for (cur_node = this_node; cur_node; cur_node = cur_node->next) {
		if (xmlIsBlankNode(cur_node) != 0)
		{
			/* If this node is blank, go to top of loop. */
			continue;
		}
        if (cur_node->type == XML_ELEMENT_NODE) {
			/* Allocates memory for an element. */
			new_element = cc_xml_new();
			/* Adds child element to this element. */
			cc_xml_setName(new_element, (char *)cur_node->name);
			/* Adds some attributes to this element. */
            for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
    			/* Allocates memory for an attribute. */
            	content_cstr = xmlGetProp(cur_node, cur_attr->name);
            	name_str = cc_string_new((char *)cur_attr->name);
            	content_str = cc_string_new(content_cstr);
            	/* Inserts an attribute to this element. */
            	cc_xml_addAttribute(new_element, name_str, content_str);
    			/* Releases memory for an attribute. */
            	xmlFree(content_cstr);
            	cc_object_release(name_str);
            	cc_object_release(content_str);
            }
			cc_xml_read_parse_node(new_element, cur_node->children);
			cc_xml_addChildElement(xml, new_element);
			/* Releases memory for an element. */
			cc_object_release(new_element);
        } else if (cur_node->type == XML_TEXT_NODE || cur_node->type == XML_CDATA_SECTION_NODE) {
        	cc_xml_setData(xml, cur_node->content);
        }
    }
	return;
}

void cc_xml_read(cc_xml *xml, char *uri)
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
		cc_xml_read_parse_node(xml, cur);
	} while(0);
	if (doc != NULL) {
		xmlFreeDoc(doc);
	}
	return;
}
void cc_xml_readBuffer(cc_xml *xml, char *buff)
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
		cc_xml_read_parse_node(xml, cur);
	} while(0);
	if (doc != NULL) {
		xmlFreeDoc(doc);
	}
	return;
}
#endif

cc_arraylist *cc_xml_getElements_getParsePath(cc_xml *xml, char *path)
{
	char *path_from;
	char *path_to;
	char *path_dup;
	cc_arraylist *path_list = NULL;
	cc_string *path_str;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		path_list = cc_arraylist_new();
		path_dup = strdup(path);

		path_from = path_dup;
		while(1)
		{
			path_to = strchr(path_from, '/');
			if (path_to == NULL)
			{
				path_str = cc_string_new(path_from);
				cc_arraylist_addAtBack(path_list, path_str);
				cc_object_release(path_str);
				break;
			}
			*path_to = 0;
			path_to ++;
			path_str = cc_string_new(path_from);
			cc_arraylist_addAtBack(path_list, path_str);
			cc_object_release(path_str);
			path_from = path_to;
		}
		free(path_dup);
	} while (0);
	return path_list;
}

cc_arraylist *cc_xml_getElements(cc_xml *xml, char *path)
{
	cc_xml_properties *properties;
	cc_arraylist *path_list;
	cc_redblacktree *element_tree;
	cc_string *path_str;
	cc_arraylist *element_list = NULL;
	cc_xml *cur_xml;
	cc_xml_properties *cur_xml_properties;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		properties = xml->properties;
		path_list = cc_xml_getElements_getParsePath(xml, path);
		element_tree = properties->elements;
		cc_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = cc_arraylist_getAtCursor(path_list);
			element_list = cc_redblacktree_get(element_tree, path_str);
			cc_object_release(path_str);
			if (element_list == NULL)
			{
				break;
			}
			cc_arraylist_setCursorAtFront(element_list);
			if (cc_arraylist_setCursorAtNext(path_list) < 0)
			{
				break;
			}
			cur_xml = cc_arraylist_getAtCursor(element_list);
			cur_xml_properties = cur_xml->properties;
			element_tree = cur_xml_properties->elements;
			cc_object_release(cur_xml);
			cc_object_release(element_list);
		}
		cc_object_release(path_list);
	} while (0);
	return element_list;
}

cc_xml *cc_xml_getElementAtFront(cc_xml *xml, char *path)
{
	cc_arraylist *get_xml_list;
	cc_xml *get_xml = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		get_xml_list = cc_xml_getElements(xml, path);
		if (get_xml_list == NULL) {
			break;
		}
		get_xml = cc_arraylist_getAtFront(get_xml_list);
		cc_object_release(get_xml_list);
	} while (0);

	return get_xml;
}

cc_xml *cc_xml_getElementAtBack(cc_xml *xml, char *path)
{
	cc_arraylist *get_xml_list;
	cc_xml *get_xml = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		get_xml_list = cc_xml_getElements(xml, path);
		if (get_xml_list == NULL) {
			break;
		}
		get_xml = cc_arraylist_getAtBack(get_xml_list);
		cc_object_release(get_xml_list);
	} while (0);

	return get_xml;
}

cc_arraylist *cc_xml_getChildElements(cc_xml *xml, char *path)
{
	cc_xml_properties *properties;
	cc_arraylist *path_list;
	cc_redblacktree *element_tree;
	cc_string *path_str;
	cc_arraylist *element_list = NULL;
	cc_arraylist *targetelement_list = NULL;
	cc_xml *cur_xml;
	cc_xml_properties *cur_xml_properties = NULL;

	do
	{
		if (xml == NULL || path == NULL)
		{
			break;
		}
		cur_xml_properties = xml->properties;
		path_list = cc_xml_getElements_getParsePath(xml, path);
		element_tree = cur_xml_properties->elements;
		cc_arraylist_setCursorAtFront(path_list);
		while (1)
		{
			path_str = cc_arraylist_getAtCursor(path_list);
			element_list = cc_redblacktree_get(element_tree, path_str);
			cc_object_release(path_str);
			if (element_list == NULL)
			{
				break;
			}
			cc_arraylist_setCursorAtFront(element_list);
			cur_xml = cc_arraylist_getAtCursor(element_list);
			cur_xml_properties = cur_xml->properties;
			targetelement_list = cur_xml_properties->elementsList;
			cc_object_grab(cur_xml_properties->elementsList);
			element_tree = cur_xml_properties->elements;
			cc_object_release(cur_xml);
			cc_object_release(element_list);
			if (cc_arraylist_setCursorAtNext(path_list) < 0)
			{
				break;
			}
		}
		cc_object_release(path_list);
	} while (0);
	return targetelement_list;
}

cc_string *cc_xml_getName(cc_xml *xml)
{
	cc_string *result_str = NULL;
	cc_xml_properties *properties;
	if (xml != NULL)
	{
		properties = xml->properties;
		cc_object_grab(properties->name);
		result_str = properties->name;
	}
	return result_str;
}

cc_string *cc_xml_getContent(cc_xml *xml)
{
	cc_string *result_str = NULL;
	cc_xml_properties *properties;
	if (xml != NULL)
	{
		properties = xml->properties;
		cc_object_grab(properties->data);
		result_str = properties->data;
	}
	return result_str;
}

cc_string *cc_xml_getAttribute(cc_xml *xml, char* name)
{
	cc_string *result_str = NULL;
	cc_xml_properties *properties;
	cc_string *name_str;
	if (xml != NULL)
	{
		properties = xml->properties;
		name_str = cc_string_new(name);
		result_str = cc_redblacktree_get(properties->attributes, name_str);
		cc_object_release(name_str);
	}
	return result_str;
}


void cc_xml_rbtree_attribute_sub(cc_object *callbackobject, cc_object *key, cc_object *object)
{
	char *key_cstr;
	char *obj_cstr;
	cc_string *key_str;
	cc_string *obj_str;
	cc_string *str1;
	cc_string *str2;
	cc_string *str3;

	str1 = cc_string_new(" ");
	str2 = cc_string_new("=\"");
	str3 = cc_string_new("\"");

	key_cstr = key->tocstring(key);
	obj_cstr = object->tocstring(object);
	key_str = cc_string_new(key_cstr);
	obj_str = cc_string_new(obj_cstr);
	cc_string_catenate(callbackobject, str1);
	cc_string_catenate(callbackobject, key_str);
	cc_string_catenate(callbackobject, str2);
	cc_string_catenate(callbackobject, obj_str);
	cc_string_catenate(callbackobject, str3);
	cc_object_release(key_str);
	cc_object_release(obj_str);
	free(key_cstr);
	free(obj_cstr);

	cc_object_release(str1);
	cc_object_release(str2);
	cc_object_release(str3);

	return;
}

void cc_xml_rbtree_element_sub(cc_object *callbackobject, cc_object *key, cc_object *object)
{
	char *obj_cstr;
	cc_string *obj_str;
	cc_xml *xml;

	cc_arraylist_setCursorAtFront(object);
	while(xml = cc_arraylist_getAtCursor(object))
	{
		obj_cstr = xml->tocstring(xml);
		obj_str = cc_string_new(obj_cstr);
		cc_string_catenate(callbackobject, obj_str);
		cc_object_release(obj_str);
		cc_object_release(xml);
		cc_arraylist_setCursorAtNext(object);
	}
	return;
}

char *cc_xml_tocstring(cc_xml *xml)
{
	cc_xml_properties *properties;
	cc_string *result_string;
	cc_string *string;
	char *cstring;

	properties = xml->properties;

	/* element name */
	result_string = cc_string_new("");
	if (cc_string_length(properties->name) != 0)
	{
		string = cc_string_new("<");
		cc_string_catenate(result_string, string);
		cc_object_release(string);
		cc_string_catenate(result_string, properties->name);

		/* attribute */
		cc_redblacktree_traversePreorder(properties->attributes, result_string, cc_xml_rbtree_attribute_sub);

		string = cc_string_new(">");
		cc_string_catenate(result_string, string);
		cc_object_release(string);

		string = cc_string_new("<![CDATA[");
		cc_string_catenate(result_string, string);
		cc_object_release(string);

		/* content */
		cc_string_catenate(result_string, properties->data);

		string = cc_string_new("]]>");
		cc_string_catenate(result_string, string);
		cc_object_release(string);
	}
	/* child elements */
	cc_redblacktree_traversePreorder(properties->elements, result_string, cc_xml_rbtree_element_sub);

	if (cc_string_length(properties->name) != 0)
	{
		string = cc_string_new("</");
		cc_string_catenate(result_string, string);
		cc_object_release(string);
		cc_string_catenate(result_string, properties->name);
		string = cc_string_new(">\n");
		cc_string_catenate(result_string, string);
		cc_object_release(string);
	}

	cstring = result_string->tocstring(result_string);
	cc_object_release(result_string);
	return cstring;
}

