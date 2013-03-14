#include "util.h"

int executionTime = 0;

void printlist(rlist *list){

  int size = 0;
  size = list->size;
  node* n = list->begin;
  char* print;
  int i;
  while( n != NULL ){
      print = n->name;
      printf( "AQUI\n\n\n\n" );
      printf( "name: %s", print );
      n = n->next;
  }
}

void initialize( rlist* p ){

    (*p).size = 0;
    (*p).begin = NULL;
    (*p).end = NULL;
}

void list_add( int seed, char *bomba ){

	node *n;
  char *key = ( char* )malloc( 64*sizeof(char) );
  char *aux = ( char* )malloc( 64*sizeof(char) );

  if( key == NULL || aux == NULL ) 
      printf( "Error creando la semilla del nodo\n" );

	n  = ( node* )malloc( sizeof( node ) );

	if( n == NULL ){
        	printf( "Error creando nodo en la lista de peticiones\n" );
		return ;
	}

	/* building the node */
	(*n).seed = seed;
	strcpy( (*n).name, bomba );

  /* Llamada a la función MD5 */
  sprintf( aux, "%d", (*n).seed ); // transform the int to char*
  strcpy( key, MDString ( aux ) ); //create his key 
  strcpy( (*n).key, key ); //copy the key in the correct value
  (*n).ticketTime = 0;
  (*n).next = NULL;

  if ((*rl).size == 0){
    (*rl).begin = n;
    (*rl).end = n;
  } else {
    (*((*rl).end)).next = n;
    (*rl).end = n;
  }

  ( *rl ).size++;

}


int list_search( rlist *r, char *name ){
	
  node* n = (node* )(*r).begin;

  while( n != NULL ){
		if( strcmp( (*n).name, name ) == 0 ) { 
      return (*n).seed;
    } 
    n = (*n).next;
	}

	return -1;
}

int check_ticket( rlist *r, char *name ){
  
  node* n = (node* )(*r).begin;

  while( n != NULL ){
    if( (strcmp( (*n).name, name ) == 0) && (executionTime < ((*n).ticketTime + 60))) { 
      return 1;
    } 
    n = (*n).next;
  }
  return 0;
}

void setInventory( int newInventory ){

    pthread_mutex_lock(&lock_inventory);
    inventory = newInventory;
    pthread_mutex_unlock(&lock_inventory);
}

int empty(){

    int i = inventory;
    if ( i >= 38000 ){
        i -= 38000;
        setInventory(i);
    }
    return i;
}

void writeInFile(char *string){

    pthread_mutex_lock(&lock_file);
    
    fprintf(file, "%s\n\n", string);

    pthread_mutex_unlock(&lock_file);
}

void *timeHandler(){

    int fill = 5*supply;

    // CLIENT *clnt;
    // int  *result_1;

    while( executionTime < timeWorking ){
        usleep( 500000 );
        executionTime += 5;
        printf("executionTime: %d\n", executionTime);
        printf("inventory: %d\n", inventory);

        if ((inventory + fill) <= maximumCapacity){
            setInventory(inventory + fill);
        } else {
            inventory = maximumCapacity;
        }
    }

    // clnt = clnt_create ("localhost", RPCENTREGA_PROG, RPCENTREGA_VERS, "udp");

    // if (clnt != NULL) {

    //       result_1 = askfortime_1(NULL, clnt);

    //   if (result_1 != (int *) NULL) {}  

    // }

}

/* This function can be used as a signal handler to terminate the
   server loop.  */
void
svc_exit (void)
{
  free (svc_pollfd);
  svc_pollfd = NULL;
  svc_max_pollfd = 0;
}

void
own_svc_run (void)
{
  int i;

  while (executionTime < timeWorking){
      struct pollfd *my_pollfd;

      if (svc_max_pollfd == 0 && svc_pollfd == NULL)
  return;

      my_pollfd = malloc (sizeof (struct pollfd) * svc_max_pollfd);
      for (i = 0; i < svc_max_pollfd; ++i)
  {
    my_pollfd[i].fd = svc_pollfd[i].fd;
    my_pollfd[i].events = svc_pollfd[i].events;
    my_pollfd[i].revents = 0;
  }

      switch (i = __poll (my_pollfd, svc_max_pollfd, -1))
  {
  case -1:
    free (my_pollfd);
    if (errno == EINTR)
      continue;
    perror (("svc_run: - poll failed"));
    return;
  case 0:
    free (my_pollfd);
    continue;
  default:
    svc_getreq_poll (my_pollfd, i);
    free (my_pollfd);
  }
    }
}