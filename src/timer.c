#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "timer.h"

#define TIMER_MAX_LABEL   64
#define TIMER_MAX_KEY     64
#define TIMER_MAX_ENTRIES 64

struct timer_entry {
    char key[TIMER_MAX_KEY];      // simple identifier for the entry
    char label[TIMER_MAX_LABEL];  // comment for the entry
    int on;           // is timer "running"
    double total;     // summarized time from all previous measurements
    double t0;        // absolute time of the current starting point
};
typedef struct timer_entry timer_entry_t;

static timer_entry_t timer_entries[TIMER_MAX_ENTRIES];
static int n_entries = 0;


/**
 * Creates new entry with short mnemonic name 'key'
 * and additional comment 'label'
 */
void timer_new_entry(char *key, char *label)
{
    int i;

    for (i = 0; i < n_entries; i++) {
        // found old entry
        if (strncmp(timer_entries[i].key, key, TIMER_MAX_KEY) == 0) {
            return;
        }
    }

    // create new entry
    // if (n_entries == TIMER_MAX_ENTRIES) {
    //     errquit("max number of timer entries exceeded (see macro TIMER_MAX_ENTRIES in src/util/timer.c)");
    // }

    strncpy(timer_entries[n_entries].key, key, TIMER_MAX_KEY);
    timer_entries[n_entries].key[TIMER_MAX_KEY - 1] = '\0';
    strncpy(timer_entries[n_entries].label, label, TIMER_MAX_LABEL);
    timer_entries[n_entries].label[TIMER_MAX_LABEL - 1] = '\0';
    timer_entries[n_entries].on = 0;
    timer_entries[n_entries].t0 = 0.0;
    timer_entries[n_entries].total = 0.0;

    n_entries++;
}


/**
 * Remove all timer entries.
 */
void timer_clear_all()
{
    n_entries = 0;
}


/**
 * Begin time measurement for the entry with mnemonic name 'key'.
 */
void timer_start(char *key)
{
    int i;

    for (i = 0; i < n_entries; i++) {
        if (strncmp(timer_entries[i].key, key, TIMER_MAX_KEY) == 0) {
            timer_entries[i].on = 1;
            timer_entries[i].t0 = abs_time();
            return;
        }
    }

    printf("key: %s\n", key);
    // errquit("unknown timer!");
}


/**
 * Stop time measurement for the entry with mnemonic name 'key'. The time
 * measured will be added to the result of the previous measurement.
 */
void timer_stop(char *key)
{
    int i;

    for (i = 0; i < n_entries; i++) {
        if (strncmp(timer_entries[i].key, key, TIMER_MAX_KEY) == 0) {
            timer_entries[i].on = 0;
            timer_entries[i].total += abs_time() - timer_entries[i].t0;
            return;
        }
    }

    printf("key: %s\n", key);
    // errquit("unknown timer!");
}


/**
 * Returns total time elapsed for the entry with mnemonic name 'key'.
 */
double timer_get(char *key)
{
    int i;

    for (i = 0; i < n_entries; i++) {
        if (strncmp(timer_entries[i].key, key, TIMER_MAX_KEY) == 0) {
            return timer_entries[i].total;
        }
    }

    printf("key: %s\n", key);
    // errquit("unknown timer!");
}


/**
 * Prints table with time statistics for all entries.
 */
void timer_stats()
{
    int i;
    double total = 0.0;

    printf("\n");
    printf(" time for (sec):\n");
    printf(" -------------------------------------------------------\n");
    for (i = 0; i < n_entries; i++) {
        printf("  %-40s%13.3f\n", timer_entries[i].label, timer_entries[i].total);
        total += timer_entries[i].total;
    }
    printf(" -------------------------------------------------------\n");
    printf("\n");
}


/**
 * Interface to the system-dependent functions for time measurements.
 */
double abs_time()
{
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    return (cur_time.tv_sec * 1000000u + cur_time.tv_usec) / 1.e6;
}
