#ifndef timeWorking
#define timeWorking 480
#endif

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"
#include <errno.h>
#include <libintl.h>
#include <sys/poll.h>
#include <rpc/rpc.h>
#include "rpcentrega.h"

#if defined(LIBC_SCCS) && !defined(lint)
static char *rcsid = "$Id: svc_run.c,v 1.2 1999/10/14 21:56:54 wsanchez Exp $";
#endif

extern pthread_mutex_t lock_file;
extern pthread_mutex_t lock_inventory;

extern FILE* file;
extern int executionTime;
extern char centerName[42];
extern int maximumCapacity;
extern int inventory;
extern int supply;
extern int timeTruck;

struct Node {
/* Every node should have a seed, a name and a hash key */
	int seed;
	char name[50];
	char key[64];
	int ticketTime;
	struct Node *next;
};

typedef struct Node node;

/*Request's list*/
struct RequestList {

	int size;
	node *begin;
	node *end;
};

typedef struct RequestList rlist;

rlist *rl;
 
void initialize( rlist* p );

void list_add( int seed, char *bomba );

int list_search( rlist *n, char *name );

int check_ticket( rlist *r, char *name );

void printlist(rlist *list);

void setInventory(int newInventory);

int empty();

void writeInFile(char *string);

void *timeHandler();

/*Codigo para rehacer la funcion de run para poder terminar el programa correctamente*/

void svc_exit (void);

void own_svc_run();