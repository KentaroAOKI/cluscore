
#include "../src/cc_hashtable.h"
#include "../src/cc_arraylist.h"
#include "../src/cc_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXCOUNT 1000000

#ifdef DEBUG
extern int cc_object_debug_alloc_count;
#endif

int main()
{
	int result;
	cc_string *str;
	cc_arraylist *list;
	char strbuff[1024];
	int index;
	struct timeval start_tm;
	struct timeval end_tm;
	struct timeval sub_tm;
	char *outstring;
	
	list = cc_arraylist_new();

	/* tests arraylist_add */
	gettimeofday(&start_tm, NULL);
	for (index=0; index < MAXCOUNT; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addAtBack(list, str);
		cc_string_dispose(str);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_addAtBack %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
//	printf("%s\n", outstring);
	free(outstring);

	gettimeofday(&start_tm, NULL);
	for (index=0; index < MAXCOUNT; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addAtFront(list, str);
		cc_string_dispose(str);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_addAtFront %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
//	printf("%s\n", outstring);
	free(outstring);

	/* tests cc_arraylist_addAt */
	gettimeofday(&start_tm, NULL);
	for (index = cc_arraylist_getLength(list) - 1; index >= 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "[%d]", index);
		str = cc_string_new(strbuff);
		cc_arraylist_addAt(list, str, index);
		cc_string_dispose(str);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_addAt %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
//	printf("%s\n", outstring);
	free(outstring);

	gettimeofday(&start_tm, NULL);
	snprintf(strbuff, sizeof(strbuff), "[%d]", MAXCOUNT);
	str = cc_string_new(strbuff);
	index = cc_arraylist_findForwardFromFront(list, str);
	cc_string_dispose(str);
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_findForwardFromFront %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	printf("index is %d\n", index);

	gettimeofday(&start_tm, NULL);
	snprintf(strbuff, sizeof(strbuff), "[%d]", MAXCOUNT);
	str = cc_string_new(strbuff);
	index = cc_arraylist_findBackwardFromBack(list, str);
	cc_string_dispose(str);
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_findBackwardFromBack %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	printf("index is %d\n", index);

	/* tests cc_arraylist_removeAt */
	gettimeofday(&start_tm, NULL);
	for (index = cc_arraylist_getLength(list) - 1; index >= 0; index -= 2) {
		cc_arraylist_removeAt(list, index);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_removeAt %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
//	printf("%s\n", outstring);
	free(outstring);
	
	/* tests cc_arraylist_removeAtFront */
	gettimeofday(&start_tm, NULL);
	for (index=0; index < MAXCOUNT; index ++) {
		cc_arraylist_removeAtFront(list);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_removeAtFront %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
//	printf("%s\n", outstring);
	free(outstring);
	
	/* tests cc_arraylist_removeAtBack */
	gettimeofday(&start_tm, NULL);
	for (index=0; index < MAXCOUNT; index ++) {
		cc_arraylist_removeAtBack(list);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("cc_arraylist_removeAtBack %d.06%d(sec)\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);
	outstring = list->tocstring(list);
	printf("%s\n", outstring);
	free(outstring);
	
	cc_arraylist_dispose(list);
#ifdef DEBUG	
	printf("cc_object_debug_alloc_count: %d\n", cc_object_debug_alloc_count);
#endif
	return result;
}
