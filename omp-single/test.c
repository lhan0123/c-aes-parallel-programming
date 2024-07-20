#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1

#include "aes.h"


static void phex(uint8_t* str);
static int test_encrypt_cbc(void);
static int test_decrypt_cbc(void);
static int textsize = 5120000 ; // define the lenght of the text that need to be encrypted or decrypted

int main(void)
{
    int exit;

#if defined(AES256)
    printf("\nTesting AES256\n\n");
#elif defined(AES192)
    printf("\nTesting AES192\n\n");
#elif defined(AES128)
    printf("\nTesting AES128\n\n");
#else
    printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    return 0;
#endif

    exit = test_encrypt_cbc() + test_decrypt_cbc() ;

    return exit;
}


// prints string as hex
static void phex(uint8_t* str)
{

#if defined(AES256)
    uint8_t len = 32;
#elif defined(AES192)
    uint8_t len = 24;
#elif defined(AES128)
    uint8_t len = 16;
#endif

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}


static int test_decrypt_cbc(void)
{
   // malloc ciphertext(in) and plaintext(out)
   uint8_t *in;
   uint8_t *out ;
   in = (uint8_t*)malloc(sizeof(uint8_t)*textsize) ;
   out = (uint8_t*)malloc(sizeof(uint8_t)*textsize) ;

   FILE* cipherfile = fopen("cipherfile", "r") ;
   fread(in, sizeof(uint8_t), textsize, cipherfile) ;
   fclose(cipherfile) ; 


#if defined(AES256)
    uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    FILE* plaintextfile = fopen("plaintext256", "r") ;
    fread(out, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ; 


#elif defined(AES192)
    uint8_t key[] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };
    FILE* plaintextfile = fopen("plaintext192", "r") ;
    fread(out, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ; 


#elif defined(AES128)
   uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    FILE* plaintextfile = fopen("plaintext128", "r") ;
    fread(out, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ; 

#endif
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };


//  uint8_t buffer[64];
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, in, textsize);
    
    printf("CBC decrypt:, ");

    if (0 == memcmp((char*) out, (char*) in, textsize)) {
        printf("SUCCESS!\n");
	return(0);
    } else {
        printf("FAILURE!\n");
	return(0);
    }
}

static int test_encrypt_cbc(void)
{
   uint8_t *in;
   uint8_t *out ;
   in = (uint8_t*)malloc(sizeof(uint8_t)*textsize) ;
   out = (uint8_t*)malloc(sizeof(uint8_t)*textsize) ;

   FILE* cipherfile = fopen("cipherfile", "r") ;
   fread(out, sizeof(uint8_t), textsize, cipherfile) ;
   fclose(cipherfile) ;

#if defined(AES256)
    uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

    FILE* plaintextfile = fopen("plaintext256", "r") ;
    fread(in, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ;
    
#elif defined(AES192)
    uint8_t key[] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };
    FILE* plaintextfile = fopen("plaintext192", "r") ;
    fread(in, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ;


#elif defined(AES128)
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    FILE* plaintextfile = fopen("plaintext128", "r") ;
    fread(in, sizeof(uint8_t), textsize, plaintextfile) ;
    fclose(plaintextfile) ;

#endif
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, in, textsize);
    printf("CBC encrypt: ");

    if (0 == memcmp((char*) out, (char*) in, textsize)) {
        printf("SUCCESS!\n");
	return(0);
    } else {
        printf("FAILURE!\n");
	return(1);
    }
}



