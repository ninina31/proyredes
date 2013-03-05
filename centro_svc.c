/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "centro.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

pthread_mutex_t lock_file;
pthread_mutex_t lock_inventory;

FILE* file;
int executionTime;
char centerName[42];
int maximumCapacity;
int inventory;
int supply;
int timeTruck;

static void
centro_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char askforsupply_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case ASKFORTIME:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) askfortime_1_svc;
		break;

	case ASKFORSUPPLY:
		_xdr_argument = (xdrproc_t) xdr_char;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) askforsupply_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	
	char fileName[ 51 ] = "log_";
    char stringToFile[256];
    int id = 0;
    void* exit_status;
    int count;

    int timeSpent = 0;
    pthread_t timeThread;

    if(argc != 11 ){
		perror("El numero de argumentos es invalido, abortando...\n");
		exit(1);
    }
    
    for( count = 0; count < 5; ++count ){
		if(strcmp(argv[count*2 + 1], "-n") == 0){
		    strcpy(centerName, argv[count*2 + 2]);
		    continue;
		} else if(strcmp(argv[count*2 + 1], "-cp") == 0){
		    maximumCapacity = atoi(argv[count*2 + 2]);
		    if(maximumCapacity < 38000 || maximumCapacity > 3800000)
			fprintf( stderr, "\nLa capacidad maxima debe estar entre 38.000 y 3.800.000 l\n" );
		    continue;
		} else if(strcmp(argv[count*2 + 1], "-i" ) == 0){
		    inventory = atoi(argv[count*2 +2]);
		    if (inventory < 0 || inventory > maximumCapacity) // consider the 0 like a positive number
			fprintf(stderr, "\nEl inventario debe estar entre 0 y la capacidad maxima\n");
			continue;
		} else if(strcmp(argv[count*2 + 1], "-t")==0){
		    timeTruck = (int)atoi(argv[ (count * 2) +2]);
		    if(timeTruck < 0 || timeTruck > 180)
			fprintf( stderr, "\nEl tiempo de respuesta debe estar entre 0 y 180 min\n");
		    continue;
		} else if(strcmp(argv[count*2 + 1], "-s")==0){
		    supply = atoi(argv[ count*2 + 2]);
		    if(supply < 0 || supply > 10000)
			fprintf( stderr, "\nEl suministro debe estar entre 0 y 10.000 l/min\n" );
		    continue;
		}
    }
	
    strcat( fileName, centerName );
    strcat( fileName, ".txt" );
    file = fopen( fileName, "w+" );

    if( file == NULL ){
    	perror( "El archivo no abrio\n" );
		exit(1);
    }

    writeInFile( "Eventos Importantes" );
    sprintf( stringToFile, "Inventario Inicial: %d l", inventory);
    writeInFile( stringToFile );

    printf("Inventario Inicial: %d l\n", inventory);

    pthread_create( &timeThread, NULL, timeHandler, NULL );

	register SVCXPRT *transp;

	pmap_unset (CENTRO_PROG, CENTRO_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, CENTRO_PROG, CENTRO_VERS, centro_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (CENTRO_PROG, CENTRO_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, CENTRO_PROG, CENTRO_VERS, centro_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (CENTRO_PROG, CENTRO_VERS, tcp).");
		exit(1);
	}

	svc_run ();

	fprintf (stderr, "%s", "svc_run returned");

	pthread_join(timeThread, &exit_status);

    fclose( file );

    printf( "Archivo de reportes listo para ser leido\n" );

    return 0;
}
