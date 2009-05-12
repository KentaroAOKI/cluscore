/*
 * ClusCore
 * cc_string
 */

#ifndef CC_STRING_H
#define CC_STRING_H

#include "cc_object.h"
#include "cc_arraylist.h"

extern int g_cc_string_object_id;
typedef cc_object cc_string;

cc_string *cc_string_new(char *string);
void cc_string_release(cc_string *string);
char *cc_string_tocstring(cc_string *string);
int cc_string_compare(cc_string *string1, cc_string *string2);
int cc_string_get_hashcode(cc_string *string, int salt);
int cc_string_length(cc_string *string);
void cc_string_catenate(cc_string *base_string, cc_object *string);
void cc_string_replase(cc_string *string, char *target_regex,
		cc_object *replase_string);
void cc_string_replase_cstring(cc_string *string, char *target_regex,
		char *replase_cstring);
void cc_string_renew(cc_string *string, char *cstring);

#endif
