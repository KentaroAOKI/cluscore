/*
 * ClusCore
 * cc_string
 */

#include "cc_string.h"
#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* memset */
#include <sys/types.h> /* regcomp */
#include <regex.h> /* regcomp */

int g_cc_string_object_id;
static const int cc_string_add_index_size = 1;

/** Constructs a cc_string_properties object which uses to the cc_object_new.
 * @param string a reference pointer of the C-Style string.
 * @return a reference pointer of the cc_string_properties object.
 */
cc_string_properties *cc_string_properties_new(char *string) {
	cc_string_properties *properties;
	int length;
	properties = malloc(sizeof(cc_string_properties));
	if (properties != NULL) {
		if (string != NULL) {
			length = strlen(string);
			properties->string = malloc(length + 1);
			memcpy(properties->string, string, length + 1);
			properties->length = length;
		} else {
			properties->string = NULL;
			properties->length = 0;
		}
		properties->nextstrings = cc_arraylist_new();
		properties->nextlength = 0;
	}
	return properties;
}

/** Destructs a cc_string_properties object which uses to the cc_object_dispose.
 * @param properties a reference pointer of the cc_string_properties object.
 */
void cc_string_properties_dispose(cc_string_properties *properties) {
	if (properties != NULL) {
		if (properties->string != NULL) {
			free(properties->string);
		}
		if (properties->nextstrings != NULL) {
			cc_arraylist_release(properties->nextstrings);
		}
		free(properties);
	}
	return;
}

/** Constructs a cc_string object.
 * @param string a reference pointer of the C-Style string.
 * @return properties a refarence pointer of the cc_string object.
 */
cc_string *cc_string_new(char *string) {
	cc_string *object = NULL;
	cc_string_properties *string_properties = cc_string_properties_new(string);
	object = cc_object_new(&g_cc_string_object_id, string_properties, cc_string_properties_dispose);
	object->tocstring = (void *)cc_string_tocstring;
	object->compare = (void *)cc_string_compare;
	return (object);
}

/** Destructs a cc_string object.
 * @param string a reference pointer of the cc_string object.
 */
void cc_string_release(cc_string *string) {
	if (string != NULL && check_object_id(string, string)) {
		cc_object_release(string);
	}
	return;
}

/** Catenates a string of the cc_strings object.
 *  @param base_string a reference of the cc_string object.
 *  @param string a reference of the cc_string object.
 */
void cc_string_catenate(cc_string *base_string, cc_object *string) {
	cc_string_properties *base_properties;
	cc_string_properties *properties;
	if (base_string != NULL && check_object_id(base_string, string) && string != NULL) {
		base_properties = base_string->properties;
		if (check_object_id(string, string)) {
			/* string object */
			properties = string->properties;
			cc_arraylist_addAtBack(base_properties->nextstrings, string);
			base_properties->nextlength = base_properties->nextlength + properties->length + properties->nextlength;
		} else {
			/* other object */
			char *object_cstring;
			cc_string *object_string;
			object_cstring = string->tocstring(string);
			object_string = cc_string_new(object_cstring);
			properties = object_string->properties;
			cc_arraylist_addAtBack(base_properties->nextstrings, object_string);
			base_properties->nextlength = base_properties->nextlength + properties->length + properties->nextlength;
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
char *cc_string_tocstring(cc_string *string) {
	int build_length;
	int total_length;
	char *cstring = NULL;
	char *tmpcstring;

	if (string != NULL && check_object_id(string, string)) {
		cc_string_properties *properties = string->properties;
		if (properties != NULL) {
			total_length = properties->length + properties->nextlength + 1;
			build_length = 0;

			cstring = malloc(total_length);
			/* build base string */
			if (properties->length > 0) {
				memcpy(cstring, properties->string, properties->length);
				build_length += properties->length;
			}

			if (((cc_arraylist_properties *)properties->nextstrings->properties)->length > 0) {
				tmpcstring = cc_arraylist_tocstring(properties->nextstrings);
				memcpy(&cstring[build_length], tmpcstring, properties->nextlength);
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
	if (string != NULL && check_object_id(string, string)) {
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
	char *cstring1;
	char *cstring2;
	cstring1 = cc_string_tocstring(string1);
	cstring2 = cc_string_tocstring(string2);
	result = strcmp(cstring1, cstring2);
	free(cstring1);
	free(cstring2);
	return result;
}

/** Replases a string of the cc_string.
 * @param string a reference pointer of the cc_string object.
 * @param target_regex a regexp for replaces a string.
 * @param replase_string a cc_string which replace string.
 */
void cc_string_replase(cc_string *string, char *target_regex, cc_object *replase_string)
{
	regex_t preg;
	regmatch_t pmatch[1];
	cc_string *work_string;
	char *build_string;
	int build_string_length;
	char *check_string;
	int check_string_length;

	if (string != NULL && check_object_id(string, string)) {
		if (regcomp(&preg, target_regex, REG_EXTENDED) != 0) {
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
		while (regexec(&preg, check_string, 1, pmatch, 0) == 0) {
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
void cc_string_replase_cstring(cc_string *string, char *target_regex, char *replase_cstring)
{
	cc_string *replase_string;
	if (string != NULL && check_object_id(string, string)) {
		replase_string = cc_string_new(replase_cstring);
		cc_string_replase(string, target_regex, replase_string);
		cc_string_release(replase_string);
	}
	return;
}

