#include <pthread.h>
#include <semaphore.h>

#ifndef _SOURCE_H_
#define _SOURCE_H_

#define BUFFER_SIZE 4096
#define BYTE_RANGE 96
#define FILE_NAME_MAX 40

typedef struct _WORDS {
   int iaOccurrenceOfWords[5];
   char szAnd[6];
   char szAt[5];
   char szIt[5];
   char szMy[5];
   char szHamlet[7];
} WORDS;

typedef struct _BUFFER {
   int count[BYTE_RANGE];
   int bytes_in_buffer;
   char szFileName[FILE_NAME_MAX + 1];
   unsigned char buffer[BUFFER_SIZE];
   WORDS *psWords;
   pthread_mutex_t mutex;
   sem_t sem_full;
   sem_t sem_empty;
} BUFFER;


////////////////////////////////FUNCTION PROTOTYPES///////////////////////////////
void CheckBufferForWords(WORDS *psWords, BUFFER *psBuffer);

void *CountBytes(void *arg);

void *ReadPdf(void *arg);

#endif