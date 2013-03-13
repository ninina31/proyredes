#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include <errno.h>
#include <libintl.h>
#include <sys/poll.h>
#include <rpc/rpc.h>

#if defined(LIBC_SCCS) && !defined(lint)
static char *rcsid = "$Id: svc_run.c,v 1.2 1999/10/14 21:56:54 wsanchez Exp $";
#endif

char* MDString (char *string);

char* makeMD5FI( int i );