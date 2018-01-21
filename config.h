#define PACKAGE "AppImageUpdaterBridge"

#define VERSION "0.0"

/* enable large inode numbers on Mac OS X 10.5 */
#ifndef _DARWIN_USE_64_BIT_INODE
#define _DARWIN_USE_64_BIT_INODE 1
#endif
