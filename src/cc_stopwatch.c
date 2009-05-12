#include "cc_stopwatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef enum cc_stopwatch_status cc_stopwatch_status;
typedef struct cc_stopwatch_properties cc_stopwatch_properties;

enum cc_stopwatch_status
{
	CC_STOPWATCH_STATUS_RESET = 0,
	CC_STOPWATCH_STATUS_START,
	CC_STOPWATCH_STATUS_STOP
};

struct cc_stopwatch_properties
{
	struct timeval start_tm;
	struct timeval stop_tm;
	cc_stopwatch_status status;
};

cc_stopwatch_properties *cc_stopwatch_properties_new(void);
void cc_stopwatch_properties_dispose(cc_stopwatch_properties *properties);

int g_cc_stopwatch_object_id;

cc_stopwatch_properties *cc_stopwatch_properties_new(void)
{
	cc_stopwatch_properties *properties = NULL;

	/* creates cc_stopwatch_properties */
	properties = malloc(sizeof(cc_stopwatch_properties));
	if (properties != NULL)
	{
		memset(properties, 0, sizeof(cc_stopwatch_properties));
		properties->status = CC_STOPWATCH_STATUS_RESET;
	}
	return properties;
}

void cc_stopwatch_properties_dispose(cc_stopwatch_properties *properties)
{
	if (properties != NULL)
	{
		/* disposes cc_stopwatch_properties */
		free(properties);
	}
	return;
}

cc_stopwatch *cc_stopwatch_new(void)
{
	cc_stopwatch *object = NULL;

	cc_stopwatch_properties *properties = cc_stopwatch_properties_new();
	object = cc_object_new(&g_cc_stopwatch_object_id, properties,
			cc_stopwatch_properties_dispose);
	object->tocstring = (void *) cc_stopwatch_tocstring;
	object->compare = (void *) cc_stopwatch_compare;
	return (object);
}

void cc_stopwatch_release(cc_stopwatch *stopwatch)
{
	cc_object_release(stopwatch);
	return;
}

void cc_stopwatch_start(cc_stopwatch *stopwatch)
{
	cc_stopwatch_properties *properties;

	if (stopwatch != NULL)
	{
		properties = stopwatch->properties;
		if (properties->status == CC_STOPWATCH_STATUS_RESET)
		{
			gettimeofday(&properties->start_tm, NULL);
			properties->status = CC_STOPWATCH_STATUS_START;
		}
		else if (properties->status == CC_STOPWATCH_STATUS_STOP)
		{
			properties->status = CC_STOPWATCH_STATUS_START;
		}
	}
	return;
}

void cc_stopwatch_stop(cc_stopwatch *stopwatch)
{
	cc_stopwatch_properties *properties;

	if (stopwatch != NULL)
	{
		properties = stopwatch->properties;
		if (properties->status == CC_STOPWATCH_STATUS_START)
		{
			gettimeofday(&properties->stop_tm, NULL);
			properties->status = CC_STOPWATCH_STATUS_STOP;
		}
	}
	return;
}

void cc_stopwatch_reset(cc_stopwatch *stopwatch)
{
	cc_stopwatch_properties *properties;

	if (stopwatch != NULL)
	{
		properties = stopwatch->properties;
		properties->status = CC_STOPWATCH_STATUS_RESET;
	}
	return;
}

char *cc_stopwatch_tocstring(cc_stopwatch *stopwatch)
{
	cc_stopwatch_properties *properties;
	struct timeval present_tm;
	struct timeval sub_tm;
	char strbuff[512];
	char *result = NULL;

	if (stopwatch != NULL)
	{
		properties = stopwatch->properties;
		switch (properties->status)
		{
		case CC_STOPWATCH_STATUS_START:
			gettimeofday(&present_tm, NULL);
			timersub(&present_tm, &properties->start_tm, &sub_tm);
			snprintf(strbuff, sizeof(strbuff), "%ld.%06ld",
					(long) sub_tm.tv_sec, (long) sub_tm.tv_usec);
			break;
		case CC_STOPWATCH_STATUS_STOP:
			timersub(&properties->stop_tm, &properties->start_tm, &sub_tm);
			snprintf(strbuff, sizeof(strbuff), "%ld.%06ld",
					(long) sub_tm.tv_sec, (long) sub_tm.tv_usec);
			break;
		case CC_STOPWATCH_STATUS_RESET:
		default:
			snprintf(strbuff, sizeof(strbuff), "%d.06%d", 0, 0);
			break;
		}
		result = strdup(strbuff);
	}
	return result;
}

int cc_stopwatch_compare(cc_stopwatch *stopwatch1, cc_stopwatch *stopwatch2)
{
	return 0;
}

