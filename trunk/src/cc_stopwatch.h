/*
 * ClusCore
 * cc_stopwatch
 */
#ifndef CC_STOPWATCH_H
#define CC_STOPWATCH_H

#include "cc_object.h"

extern int g_cc_stopwatch_object_id;
typedef cc_object cc_stopwatch;

cc_stopwatch *cc_stopwatch_new(void);
void cc_stopwatch_release(cc_stopwatch *stopwatch);
void cc_stopwatch_start(cc_stopwatch *stopwatch);
void cc_stopwatch_stop(cc_stopwatch *stopwatch);
void cc_stopwatch_reset(cc_stopwatch *stopwatch);
char *cc_stopwatch_tocstring(cc_stopwatch *stopwatch);
int cc_stopwatch_compare(cc_stopwatch *stopwatch1, cc_stopwatch *stopwatch2);

#endif
