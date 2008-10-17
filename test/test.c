
#include "../src/cc_hashtable.h"
#include "../src/cc_arraylist.h"
#include "../src/cc_string.h"
#include "../src/cc_stopwatch.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXCOUNT 1000000

#ifdef DEBUG
extern int cc_object_debug_alloc_count;
#endif

int main()
{
	int result;
	cc_string *str;
	cc_arraylist *list;
	cc_hashtable *table;
	cc_stopwatch *stopwatch;
	char strbuff[1024];
	int index;
	char *outstring;
	
	stopwatch = cc_stopwatch_new();

	list = cc_arraylist_new();
	/*
	 *  tests cc_arraylist_addAtBack
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=100; index > 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "%d/", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addWithSort(list, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_arraylist_addWithSort %s(sec)\n", outstring);
	free(outstring);
/*	
	outstring = list->tocstring(list);
	printf("%s\n", outstring);
	free(outstring);
*/	
	cc_arraylist_release(list);
	
	list = cc_arraylist_new();

	/*
	 *  tests cc_arraylist_addAtBack
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addAtBack(list, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_arraylist_addAtBack x %d: %s(sec)\n", MAXCOUNT, outstring);
	free(outstring);
/*	
	outstring = list->tocstring(list);
	printf("%s\n", outstring);
	free(outstring);
*/
	/*
	 *  tests cc_arraylist_addAt
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index = MAXCOUNT - 1; index >= 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "(%d)", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addAt(list, str, index);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);

	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_arraylist_addAt x %d: %s(sec)\n", MAXCOUNT, outstring);
	free(outstring);
/*
	outstring = list->tocstring(list);
	printf("%s\n", outstring);
	free(outstring);
*/	
	/*
	 *  tests cc_arraylist_findForwardFromFront
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	snprintf(strbuff, sizeof(strbuff), "%d", MAXCOUNT - 1);
	str = cc_string_new(strbuff);
	for (index=0; index < 10; index ++) {
		result = cc_arraylist_findForwardFromFront(list, str);
	}
	cc_string_release(str);
	cc_stopwatch_stop(stopwatch);

	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_arraylist_findForwardFromFront %s(sec)\n", outstring);
	free(outstring);

	printf("index is %d\n", result);
	
	cc_arraylist_release(list);

#ifdef DEBUG	
	printf("cc_object_debug_alloc_count: %d\n", cc_object_debug_alloc_count);
#endif
	
	
	table = cc_hashtable_new(512);
	
	/*
	 *  tests cc_hashtable_set
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d", index);
		str = cc_string_new(strbuff);
		cc_hashtable_set(table, strbuff, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_hashtable_set %s(sec)\n", outstring);
	free(outstring);
	
	outstring = table->tocstring(table);
//	printf("%s\n", outstring);
	free(outstring);

	/*
	 *  tests cc_hashtable_set
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index = MAXCOUNT - 1; index >= 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "(%d)", index);
		str = cc_string_new(strbuff);
		cc_hashtable_set(table, strbuff, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);

	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_hashtable_set %s(sec)\n", outstring);
	free(outstring);

	outstring = table->tocstring(table);
//	printf("%s\n", outstring);
	free(outstring);
	
	/*
	 *  tests cc_arraylist_findForwardFromFront
	 */
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	snprintf(strbuff, sizeof(strbuff), "%d", MAXCOUNT - 1);
	for (index=0; index < 10; index ++) {
		str = cc_hashtable_get(table, strbuff);
		cc_string_release(str);
	}
	str = cc_hashtable_get(table, strbuff);
	cc_stopwatch_stop(stopwatch);

	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_arraylist_findForwardFromFront %s(sec)\n", outstring);
	free(outstring);

	outstring = str->tocstring(str);
	printf("%s is %s\n", strbuff, outstring);
	free(outstring);
	cc_string_release(str);
	
	cc_hashtable_release(table);
	
#ifdef DEBUG	
	printf("cc_object_debug_alloc_count: %d\n", cc_object_debug_alloc_count);
#endif
	return result;
}
