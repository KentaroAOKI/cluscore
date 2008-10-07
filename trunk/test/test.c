
#include "../src/cc_hashtable.h"
#include "../src/cc_arraylist.h"
#include "../src/cc_string.h"
#include <stdio.h>

int main()
{
	int result;
	cc_string *str;
	cc_arraylist *list;
	char strbuff[1024];
	int index;

	list = cc_arraylist_new();
	for (index=0; index < 1000; index ++) {
		snprintf(strbuff, sizeof(strbuff), "%d/", index);
		str = cc_string_new(strbuff);
		cc_arraylist_add(list, str);
		cc_string_dispose(str);
	}
	for (index=1000; index > 0; index --) {
		snprintf(strbuff, sizeof(strbuff), "(%d)/", index);
		str = cc_string_new(strbuff);
		cc_arraylist_insertWithIndex(list, str, index);
		cc_string_dispose(str);
	}
	printf("%s\n", list->tocstring(list));
	cc_arraylist_dispose(list);
	return result;
}
