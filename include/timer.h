#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

void timer_new_entry(char *key, char *label);

void timer_clear_all();

void timer_start(char *key);

void timer_stop(char *key);

double timer_get(char *key);

void timer_stats();

double abs_time();

#endif /* TIMER_H_INCLUDED */