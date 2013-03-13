#include "crypto.h"

#ifndef MD
#define MD 5
#endif

#if MD == 2
#include "md2.h"
#endif
#if MD == 4
#include "md4.h"
#endif
#if MD == 5
#include "md5.h"
#endif

#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000

char* MDString PROTO_LIST ((char *));
static void MDTimeTrial PROTO_LIST ((void));
static void MDTestSuite PROTO_LIST ((void));
static void MDFile PROTO_LIST ((char *));
static void MDFilter PROTO_LIST ((void));
static void MDPrint PROTO_LIST ((unsigned char [16]));

#if MD == 2
#define MD5_CTX MD2_CTX
#define MDInit MD2Init
#define MDUpdate MD2Update
#define MDFinal MD2Final
#endif
#if MD == 4
#define MD5_CTX MD4_CTX
#define MDInit MD4Init
#define MDUpdate MD4Update
#define MDFinal MD4Final
#endif
#if MD == 5
#define MD5_CTX MD5_CTX
#define MDInit MD5Init
#define MDUpdate MD5Update
#define MDFinal MD5Final
#endif

char* MDString (char *string){

  MD5_CTX context;
  static unsigned char digest[64];
  unsigned int len = strlen (string);

  MDInit (&context);
  MDUpdate (&context, string, len);
  MDFinal (digest, &context);

  return digest;
}

char* makeMD5FI( int i ){

	char* key = malloc(sizeof(char)*64);
	char* aux;
	sprintf( aux, "%d", i);

	strcpy( key,MDString( aux ));

	return key;
}