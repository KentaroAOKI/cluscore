
#include <cluscore/cc_hashtable.h>
#include <cluscore/cc_arraylist.h>
#include <cluscore/cc_redblacktree.h>
#include <cluscore/cc_string.h>
#include <cluscore/cc_stopwatch.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCOUNT 1000000

#ifdef LIBCLUSCORE_DEBUG
//extern int cc_object_debug_alloc_count;
#endif

int main()
{
	int result;
	cc_string *str;
	cc_string *rstr;
	cc_object *result_obj;
	cc_arraylist *list;
	cc_hashtable *table;
	cc_stopwatch *stopwatch;
	char strbuff[1024];
	int index;
	char *outstring;
	cc_redblacktree *tree;

#if 1

#ifdef LIBCLUSCORE_DEBUG
//	printf("cc_object = %d\n", cc_object_debug_alloc_count);
#endif
	stopwatch = cc_stopwatch_new();

	tree = cc_redblacktree_new();
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		outstring = malloc(100);
		free(outstring);
		outstring = malloc(100);
		free(outstring);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("malloc %s(sec)\n", outstring);
	free(outstring);
#endif

#if 1

	stopwatch = cc_stopwatch_new();

	tree = cc_redblacktree_new();
	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		sprintf(strbuff, "%08d", index);
		str = cc_string_new(strbuff);
		cc_redblacktree_insert(tree, str, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_redblacktree_insert %s(sec)\n", outstring);
	free(outstring);

	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		sprintf(strbuff, "%08d", index);
		str = cc_string_new(strbuff);
		rstr = cc_redblacktree_get(tree, str);
		cc_string_release(rstr);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_redblacktree_get %s(sec)\n", outstring);
	free(outstring);

	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		sprintf(strbuff, "%08d", index);
		str = cc_string_new(strbuff);
		cc_redblacktree_delete(tree, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_redblacktree_delete %s(sec)\n", outstring);
	free(outstring);

	cc_redblacktree_release(tree);
	cc_stopwatch_release(stopwatch);
#endif

#ifdef DEBUG
	printf("cc_object_debug_alloc_count: %d\n", cc_object_debug_alloc_count);
#endif

	stopwatch = cc_stopwatch_new();
	table = cc_hashtable_new(1000000);

	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		sprintf(strbuff, "%08d", index);
		str = cc_string_new(strbuff);
		cc_hashtable_set(table, str, str);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_hashtable_set %s(sec) %d(collision)\n", outstring, cc_hashtable_collision(table));
	free(outstring);

	cc_stopwatch_reset(stopwatch);
	cc_stopwatch_start(stopwatch);
	for (index=0; index < MAXCOUNT; index ++)
	{
		sprintf(strbuff, "%08d", index);
		str = cc_string_new(strbuff);
		rstr = cc_hashtable_get(table, str);
#if 0
		outstring = rstr->tocstring(rstr);
		printf("%s=%s\n", strbuff, outstring);
		free(outstring);
#endif
		cc_string_release(rstr);
		cc_string_release(str);
	}
	cc_stopwatch_stop(stopwatch);
	outstring = cc_stopwatch_tocstring(stopwatch);
	printf("cc_hashtable_get %s(sec)\n", outstring);
	free(outstring);


	cc_hashtable_release(table);
	cc_stopwatch_release(stopwatch);


	exit(0);



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
