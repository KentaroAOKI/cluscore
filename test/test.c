
#include "../src/cc_hashtable.h"
#include "../src/cc_arraylist.h"
#include "../src/cc_string.h"
#include <stdio.h>
#include <sys/time.h>

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
	
	/* tests arraylist_add */
	gettimeofday(&start_tm, NULL);
	list = cc_arraylist_new();
	for (index=0; index < 100000; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d/", index);
		str = cc_string_new(strbuff);
		cc_arraylist_add(list, str);
		cc_string_dispose(str);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("phase1 %d.%d\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);

	/* tests arraylist_insert */
	gettimeofday(&start_tm, NULL);
	for (index=100000; index > 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "(%d)/", index);
		str = cc_string_new(strbuff);
		cc_arraylist_insertWithIndex(list, str, index);
		cc_string_dispose(str);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("phase2 %d.%d\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);

	/* tests arraylist_remove */
	gettimeofday(&start_tm, NULL);
	for (index=200000; index > 0; index -= 2) {
		cc_arraylist_removeWithIndex(list, index);
	}
	gettimeofday(&end_tm, NULL);
	timersub(&end_tm, &start_tm, &sub_tm);
	printf("phase3 %d.%d\n", (int)sub_tm.tv_sec, (int)sub_tm.tv_usec);

	printf("%s\n", list->tocstring(list));
	cc_arraylist_dispose(list);
	return result;
}
