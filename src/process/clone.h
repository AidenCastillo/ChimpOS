#include <stdint.h>

#define CLONE_VM        0x00000100  /* set if VM shared between processes */
#define CLONE_FS        0x00000200  /* set if fs info shared between processes */
#define CLONE_FILES     0x00000400  /* set if open files shared between processes */
#define CLONE_SIGHAND   0x00000800  /* set if signal handlers and blocked signals shared */
#define CLONE_THREAD    0x00010000  /* Same thread group? */
#define CLONE_PARENT_SETTID 0x00100000 /* set the TID in the parent */
#define CLONE_CHILD_CLEARTID 0x00200000 /* clear the TID in the child */

// link to clone.s
extern int clone(void (*fn)(void), void* stack, int flags, void* arg, void* ptid, void* ctid);
