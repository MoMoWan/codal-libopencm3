#ifndef CM_TASKS_H
#define CM_TASKS_H

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

int start_background_tasks(void);
int restart_callback(void);

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  CM_TASKS_H
