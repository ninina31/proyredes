/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "centro.h"
#include "util.h"

int *
askfortime_1_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;

	char stringToFile[256];
    int timeT = executionTime;   

    printf("Me pidieron tiempo\n");

    if( inventory == maximumCapacity ){
		sprintf( stringToFile, "Tanque Lleno: %d minutos", executionTime );
		writeInFile(stringToFile);
    }
	    
	result = timeT;

	return &result;
}

int *
askforsupply_1_svc(char *argp, struct svc_req *rqstp)
{
	static int  result;

	char* nameBomba = argp;

	char stringToFile[256];
    int i = 0;

    printf("Me pidieron suministro\n");

    if( inventory == maximumCapacity ){
		sprintf( stringToFile, "Tanque Lleno: %d minutos", executionTime );
		writeInFile(stringToFile);
    }
	
	printf("Cliente solicito suministro\n");
	    
	i = inventory;
	
	if( i >= 38000 ){
	    printf("Tengo suficiente inventario, despachando...\n");
	    empty();
	    sprintf( stringToFile, "Suministro: %d min, %s, OK, %d l", executionTime, nameBomba, inventory );
	    writeInFile(stringToFile);
	    i = inventory;
		
	    if( i == 0 ){
			sprintf( stringToFile, "Tanque Vacio: %d min", executionTime );
			writeInFile(stringToFile);
	    }
	    result = 0;

	} else {
	    printf("No tengo suficiente inventario\n");
	    sprintf( stringToFile, "Suministro: %d min, %s, Sin inventario, %d l", executionTime, nameBomba,inventory );
	    writeInFile(stringToFile);
	    result = 1;
	}

	return &result;
}
