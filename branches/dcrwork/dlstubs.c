#include <sys/types.h>
#include <dlfcn.h>

/* dl*() stub routines for static compilation.  Prepared from
   /usr/include/dlfcn.h by Hal Pomeranz <hal@deer-run.com> */
/*030131 mgp crafted this version for RedHat 8 linux*/

void *dlopen(__const char *__file, int __mode) {}
void *dlsym(void *ptr, const char *str) {}
int dlclose(void *__handle) {}
char *dlerror(void) {}

void *_dlsym(void *__restrict __handle,
                    __const char *__restrict __name) {}
char *_dlerror(void) {}

/*more from http://www.deer-run.com/~hal/sol-static.txt */
/* To explain avoiding unresolved symbols doing a static compile*/ 
/*...various dl*() functions are used to control dynamic loading...*/
/*... since we're attempting to build a statically linked binary, the various*/
/* dl*() routiines should never be called in the first place.  This means we */
/*could write our own stub routines and link them into our binary. */
/*...The stub functions were simply ripped directly from the prototypes */
/* given in /usr/include/dlfcn.h */

