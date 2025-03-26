#ifndef TIMER_H
#define TIMER_H

void timer_action(const char *uuid, const char *action);

void timer_delete(const char *uuid);

void timer_add(const char *data);

#endif
