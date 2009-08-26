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

#include "cc_string.h"
#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* memset */
#include <sys/types.h> /* regcomp */
#include <regex.h> /* regcomp */

int g_cc_string_object_id;
typedef struct cc_string_properties cc_string_properties;
struct cc_string_properties
{
	int length;
	int nextlength;
	cc_arraylist *nextstrings;
	char string[1];
};

/** Constructs a cc_string_properties object which uses to the cc_object_new.
 * @param string a reference pointer of the C-Style string.
 * @return a reference pointer of the cc_string_properties object.
 */
#if 0
cc_string_properties *cc_string_properties_new(char *string)
{
	cc_string_properties *properties;
	int length;
	properties = malloc(sizeof(cc_string_properties));
	if (properties != NULL)
	{
		if (string != NULL)
		{
			length = strlen(string);
			properties->string = malloc(length + 1);
			memcpy(properties->string, string, length + 1);
			properties->length = length;
		}
		else
		{
			properties->string = NULL;
			properties->length = 0;
		}
		properties->nextstrings = NULL;
		properties->nextlength = 0;
	}
	return properties;
}
#else if
cc_string_properties *cc_string_properties_new(char *string)
{
	cc_string_properties *properties;
	int length;
	if (string != NULL)
	{
		length = strlen(string);
	}
	else
	{
		length = 0;
	}
	properties = malloc(sizeof(cc_string_properties) + length);
	if (properties != NULL)
	{
		if (string != NULL)
		{
			memcpy(properties->string, string, length + 1);
			properties->length = length;
		}
		else
		{
			properties->string[0] = 0;
			properties->length = 0;
		}
		properties->nextstrings = NULL;
		properties->nextlength = 0;
	}
	return properties;
}

cc_string_properties *cc_string_properties_newWithLength(char *string, int length)
{
	cc_string_properties *properties;
	if (string == NULL)
	{
		length = 0;
	}
	properties = malloc(sizeof(cc_string_properties) + length);
	if (properties != NULL)
	{
		if (string != NULL)
		{
			memcpy(properties->string, string, length);
			properties->string[length] = 0;
			properties->length = length;
		}
		else
		{
			properties->string[0] = 0;
			properties->length = 0;
		}
		properties->nextstrings = NULL;
		properties->nextlength = 0;
	}
	return properties;
}

inline void cc_string_properties_initialize(cc_string_properties *properties,
		char *string, int length)
{
	if (string != NULL)
	{
		memcpy(properties->string, string, length + 1);
		properties->length = length;
	}
	else
	{
		properties->string[0] = 0;
		properties->length = 0;
	}
	properties->nextstrings = NULL;
	properties->nextlength = 0;
	return;
}
#endif

/** Destructs a cc_string_properties object which uses to the cc_object_dispose.
 * @param properties a reference pointer of the cc_string_properties object.
 */
#if 1
void cc_string_properties_dispose(cc_string_properties *properties)
{
	if (properties != NULL)
	{
		//		if (properties->string != NULL) {
		//			free(properties->string);
		//		}
		if (properties->nextstrings != NULL)
		{
			cc_arraylist_release(properties->nextstrings);
		}
		free(properties);
	}
	return;
}
#else if
void cc_string_properties_dispose(cc_string_properties *properties)
{
	if (properties != NULL)
	{
		//		if (properties->string != NULL) {
		//			free(properties->string);
		//		}
		if (properties->nextstrings != NULL)
		{
			cc_arraylist_release(properties->nextstrings);
		}
	}
	return;
}
#endif

/** Constructs a cc_string object.
 * @param string a reference pointer of the C-Style string.
 * @return properties a refarence pointer of the cc_string object.
 */
#if 1
cc_string *cc_string_new(char *string)
{
	cc_string *object = NULL;
	cc_string_properties *string_properties = cc_string_properties_new(string);
	object = cc_object_new(&g_cc_string_object_id, string_properties,
			cc_string_properties_dispose);
	object->tocstring = (void *) cc_string_tocstring;
	object->compare = (void *) cc_string_compare;
	object->get_hashcode = (void *) cc_string_get_hashcode;
	return (object);
}
cc_string *cc_string_newWithLength(char *string, int length)
{
	cc_string *object = NULL;
	cc_string_properties *string_properties = cc_string_properties_newWithLength(string, length);
	object = cc_object_new(&g_cc_string_object_id, string_properties,
			cc_string_properties_dispose);
	object->tocstring = (void *) cc_string_tocstring;
	object->compare = (void *) cc_string_compare;
	object->get_hashcode = (void *) cc_string_get_hashcode;
	return (object);
}
#else if
cc_string *cc_string_new(char *string)
{
	cc_string *object = NULL;
	int length;

	if (string != NULL)
	{
		length = strlen(string);
	}
	else
	{
		length = 0;
	}
	object = cc_object_new2(&g_cc_string_object_id, sizeof(cc_string_properties) + length, cc_string_properties_dispose);
	if (object != NULL)
	{
		cc_string_properties_initialize(object->properties, string, length);
		object->tocstring = (void *)cc_string_tocstring;
		object->compare = (void *)cc_string_compare;
		object->get_hashcode = (void *)cc_string_get_hashcode;
	}
	return (object);
}
#endif
/** Destructs a cc_string object.
 * @param string a reference pointer of the cc_string object.
 */
void cc_string_release(cc_string *string)
{
	if (string != NULL && cc_object_check_id(string, string))
	{
		cc_object_release(string);
	}
	return;
}

/** Catenates a string of the cc_strings object.
 *  @param base_string a reference of the cc_string object.
 *  @param string a reference of the cc_string object.
 */
void cc_string_catenate(cc_string *base_string, cc_object *string)
{
	cc_string_properties *base_properties;
	cc_string_properties *properties;
	if (base_string != NULL && string != NULL && cc_object_check_id(base_string, string))
	{
		base_properties = base_string->properties;
		if (base_properties->nextstrings == NULL)
		{
			base_properties->nextstrings = cc_arraylist_new();
		}
		if (cc_object_check_id(string, string))
		{
			/* string object */
			properties = string->properties;
			cc_arraylist_addAtBack(base_properties->nextstrings, string);
			base_properties->nextlength = base_properties->nextlength
					+ properties->length + properties->nextlength;
		}
		else
		{
			/* other object */
			char *object_cstring;
			cc_string *object_string;
			object_cstring = string->tocstring(string);
			object_string = cc_string_new(object_cstring);
			properties = object_string->properties;
			cc_arraylist_addAtBack(base_properties->nextstrings, object_string);
			base_properties->nextlength = base_properties->nextlength
					+ properties->length + properties->nextlength;
			cc_string_release(object_string);
			free(object_cstring);
		}
	}
	return;
}

/** Generates a C-style string.
 * @param string a reference pointer of the cc_string object.
 * @return Allocated a C-style string.
 */
char *cc_string_tocstring(cc_string *string)
{
	int build_length;
	int total_length;
	char *cstring = NULL;
	char *tmpcstring;

	if (string != NULL && cc_object_check_id(string, string))
	{
		cc_string_properties *properties = string->properties;
		if (properties != NULL)
		{
			total_length = properties->length + properties->nextlength + 1;
			build_length = 0;

			cstring = malloc(total_length);
			/* build base string */
			if (properties->length > 0)
			{
				memcpy(cstring, properties->string, properties->length);
				build_length += properties->length;
			}

			if ((cc_arraylist_properties *)properties->nextlength > 0)
			{
				tmpcstring = cc_arraylist_tocstring(properties->nextstrings);
				memcpy(&cstring[build_length], tmpcstring,
						properties->nextlength);
				free(tmpcstring);
			}

			build_length += properties->nextlength;
			cstring[build_length] = 0;
		}
	}
	return (cstring);
}

/** Calculates a length of the cc_string.
 * @param string a reference pointer of the cc_string object.
 * @return length.
 */
int cc_string_length(cc_string *string)
{
	int length = -1;
	if (string != NULL && cc_object_check_id(string, string))
	{
		cc_string_properties *properties = string->properties;
		length = properties->length + properties->nextlength;
	}
	return (length);
}

/** Compares the cc_string.
 * @param string1 a reference pointer of the cc_string object.
 * @param string2 a reference pointer of the cc_string object.
 * @return a result such as strcmp.
 */
int cc_string_compare(cc_string *string1, cc_string *string2)
{
	int result;
	cc_string_properties *properties1;
	cc_string_properties *properties2;
	char *cstring1;
	char *cstring2;
	char *curcstring1;
	char *curcstring2;

	properties1 = string1->properties;
	properties2 = string2->properties;
	if (properties1->nextlength == 0)
	{
		cstring1 = NULL;
		curcstring1 = properties1->string;
	}
	else
	{
		cstring1 = cc_string_tocstring(string1);
		curcstring1 = cstring1;
	}
	if (properties2->nextlength == 0)
	{
		cstring2 = NULL;
		curcstring2 = properties2->string;
	}
	else
	{
		cstring2 = cc_string_tocstring(string2);
		curcstring2 = cstring2;
	}
	/* strcmp */
	result = strcmp((const char*) curcstring1, (const char*) curcstring2);

	if (cstring1 != NULL)
	{
		free(cstring1);
	}
	if (cstring2 != NULL)
	{
		free(cstring2);
	}
	return result;
}

/** Calculates a hash-code of the cc_string.
 * @param string1 a reference pointer of the cc_string object.
 * @param string2 a reference pointer of the cc_string object.
 * @return a result such as strcmp.
 */
int cc_string_get_hashcode(cc_string *string, int salt)
{
	cc_string_properties *properties;
	int hashvalue;
	char *cstr;
	char *cstr_cursor;
	int mallocflg = 0;

	properties = string->properties;
	if (properties->nextlength == 0)
	{
		cstr = properties->string;
	}
	else
	{
		cstr = cc_string_tocstring(string);
		mallocflg = 1;
	}
	cstr_cursor = cstr;
	hashvalue = salt;
	while (*cstr_cursor++)
	{
		hashvalue = hashvalue * 37 + (unsigned char) *cstr_cursor;
	}
	if (mallocflg != 0)
	{
		free(cstr);
	}
	return hashvalue;
}

/** Replases a string of the cc_string.
 * @param string a reference pointer of the cc_string object.
 * @param target_regex a regexp for replaces a string.
 * @param replase_string a cc_string which replace string.
 */
void cc_string_replase(cc_string *string, char *target_regex,
		cc_object *replase_string)
{
	regex_t preg;
	regmatch_t pmatch[1];
	cc_string *work_string;
	char *build_string;
	int build_string_length;
	char *check_string;
	int check_string_length;

	if (string != NULL && cc_object_check_id(string, string))
	{
		if (regcomp(&preg, target_regex, REG_EXTENDED) != 0)
		{
			return;
		}
		build_string = cc_string_tocstring(string);
		build_string_length = cc_string_length(string);
		/* dispose properties of cc_string. and create it. */
		cc_string_properties_dispose(string->properties);
		string->properties = cc_string_properties_new("");
		/* do replace */
		check_string = build_string;
		check_string_length = build_string_length;
		while (regexec(&preg, check_string, 1, pmatch, 0) == 0)
		{
			/* fetch string to match string before. */
			check_string[pmatch[0].rm_so] = 0;
			work_string = cc_string_new(check_string);
			cc_string_catenate(string, work_string);
			cc_string_release(work_string);
			/* catenates replase_string. */
			cc_string_catenate(string, replase_string);
			/* changes a status of string. */
			check_string = check_string + pmatch[0].rm_eo;
			check_string_length -= pmatch[0].rm_eo;
			//printf("%d - %d(%d)\n", pmatch[0].rm_so, pmatch[0].rm_eo, check_string_length);
		}
		/* catenates last string. */
		work_string = cc_string_new(check_string);
		cc_string_catenate(string, work_string);
		cc_string_release(work_string);

		free(build_string);
		regfree(&preg);
	}
	return;
}

/** Replases a string of the cc_string by C-style string.
 * @param string a reference pointer of the cc_string object.
 * @param target_regex a regexp for replaces a string.
 * @param replase_Cstring a C-style string which replace string.
 */
void cc_string_replase_cstring(cc_string *string, char *target_regex,
		char *replase_cstring)
{
	cc_string *replase_string;
	if (string != NULL && cc_object_check_id(string, string))
	{
		replase_string = cc_string_new(replase_cstring);
		cc_string_replase(string, target_regex, replase_string);
		cc_string_release(replase_string);
	}
	return;
}

