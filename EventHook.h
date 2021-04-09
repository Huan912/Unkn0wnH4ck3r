#ifndef EVENTHOOK_H
#define EVENTHOOK_H
void HookEvent(char* name, void (*pfnEvent)(struct event_args_s* args));
#endif