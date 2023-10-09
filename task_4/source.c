#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "source.h"

int main(int argc, char *argv[]) {
   unsigned long iFileNameLength;
   pthread_t threadA, threadB;

   // Checks for argument given from user.
   if (argc < 2) {
      printf("Error, No filename argument given.\n");
      return 1;
   }
   iFileNameLength = strlen(argv[1]);
   if (iFileNameLength > FILE_NAME_MAX) {
      printf("Filename not accepted!\n");
      return 1;
   }

   // Allocate space for buffer and words struct.
   BUFFER *psBuffer = (BUFFER *) malloc(sizeof(BUFFER));
   WORDS *psWords = (WORDS *) malloc(sizeof(WORDS));

   // Clean sheets
   memset(psWords, 0, sizeof(WORDS));
   memset(psBuffer,0, sizeof(BUFFER));

   // Init semaphores
   sem_init(&(psBuffer->sem_full), 0, 0);
   sem_init(&(psBuffer->sem_empty), 0, 1);

   // Init mutex.
   pthread_mutex_init(&(psBuffer->mutex), 0);

   // Copy filename into struct.
   strncpy(psBuffer->szFileName, argv[1], iFileNameLength);
   // Ensure zero terminator.
   psBuffer->szFileName[FILE_NAME_MAX] = '\0';

   // Set values in words.
   strncpy(psWords->szAnd, " and ", 5);
   psWords->szAnd[5] = '\0';
   strncpy(psWords->szAt, " at ", 4);
   psWords->szAt[4] = '\0';
   strncpy(psWords->szIt, " it ", 4);
   psWords->szIt[4] = '\0';
   strncpy(psWords->szMy, " my ", 4);
   psWords->szMy[4] = '\0';
   strncpy(psWords->szHamlet, "Hamlet", 6);
   psWords->szHamlet[6] = '\0';

   // Set pointer in buffer to point to allocated Words struct.
   psBuffer->psWords = psWords;

   // Create thread to read file.
   if (pthread_create(&threadA, NULL, ReadPdf, (void *) psBuffer) != 0) {
      perror("Could not create thread A");
      exit(1);
   }
   // Create thread to count bytes.
   if (pthread_create(&threadB, NULL, CountBytes, (void *) psBuffer) != 0) {
      perror("Could not create thread B");
      exit(1);
   }
   // Join first thread to main thread when it is done.
   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(1);
   }
   // Join second thread to main thread when it is done.
   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(1);
   }

   // Cleanup.
   sem_destroy(&(psBuffer->sem_empty));
   sem_destroy(&(psBuffer->sem_full));
   pthread_mutex_destroy(&(psBuffer->mutex));
   free(psWords);
   free(psBuffer);

   return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
// * Compare words from buffer with the words in WORDS and                         //
//   increment the index representing the word compared.                           //
// * checks if there is space left in buffer to check whole word before comparing. //
/////////////////////////////////////////////////////////////////////////////////////
void CheckBufferForWords(WORDS *psWords, BUFFER *psBuffer) {


   for (int i = 0; i < psBuffer->bytes_in_buffer; i++) {
      
      if (i < BUFFER_SIZE - 6) {
         // Check AND.
         if (strncasecmp((const char *) psBuffer->buffer + i, psWords->szAnd, 5) == 0) {
            psWords->iaOccurrenceOfWords[0]++;
            i = i + 5;
         }
      }
      
      if (i < BUFFER_SIZE - 5) {
         // Check AT.
         if (strncasecmp((const char *) psBuffer->buffer + i, psWords->szAt, 4) == 0) {
            psWords->iaOccurrenceOfWords[1]++;
            i = i + 4;
            // Check IT.
         } else if (strncasecmp((const char *) psBuffer->buffer + i, psWords->szIt, 4) == 0) {
            psWords->iaOccurrenceOfWords[2]++;
            i = i + 4;
            // Check MY.
         } else if (strncasecmp((const char *) psBuffer->buffer + i, psWords->szMy, 4) == 0) {
            psWords->iaOccurrenceOfWords[3]++;
            i = i + 4;
         }
      }
      if (i < BUFFER_SIZE - 7) {
         // Check HAMLET.
         if (strncasecmp((const char *) psBuffer->buffer + i, psWords->szHamlet, 6) == 0) {
            psWords->iaOccurrenceOfWords[4]++;
            i = i + 6;
         }
      }


   }

}

void *ReadPdf(void *arg) {
   // Cast the argument from void pointer to BUFFER and use it to access/change values.
   BUFFER *psBuffer = (BUFFER *) arg;

   FILE *fp = fopen(psBuffer->szFileName, "rb");
   if (!fp) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {

      // Wait until buffer is empty (signal that all bytes are counted).
      sem_wait(&(psBuffer->sem_empty));
      // Lock the mutex so nothing else can lock it.
      pthread_mutex_lock(&(psBuffer->mutex));


      // Read upto (4096 - eventually bytes not counted in buffer) and update read_bytes with the amount bytes read.
      int read_bytes = fread(psBuffer->buffer + psBuffer->bytes_in_buffer, 1,
                             BUFFER_SIZE - psBuffer->bytes_in_buffer,
                             fp);
      psBuffer->bytes_in_buffer += read_bytes;

      // If there are read less than 4096 bytes we can assume the last block is read.
      if (read_bytes < BUFFER_SIZE - psBuffer->bytes_in_buffer) {
         // Signal ready to be counted.
         sem_post(&(psBuffer->sem_full));
         // Unlock mutex.
         pthread_mutex_unlock(&(psBuffer->mutex));
         break;
      }
      // Signal that the buffer is full and ready to be counted.
      sem_post(&(psBuffer->sem_full));
      // Unlock mutex so it can be used by others.
      pthread_mutex_unlock(&(psBuffer->mutex));
   }
   // Close file.
   fclose(fp);
   // Exit thread and return null to the join.
   pthread_exit(NULL);
}

void *CountBytes(void *arg) {

   unsigned char cHolder;
   // Cast the argument from void pointer to BUFFER and use it to access/change values.
   BUFFER *psBuffer = (BUFFER *) arg;
   // Clean sheets
   memset(psBuffer->count, 0, sizeof(psBuffer->count));

   while (1) {

      // Wait here until sem_empty  (buffer is filled up with new bytes).
      sem_wait(&(psBuffer->sem_full));

      // Lock mutex.
      pthread_mutex_lock(&(psBuffer->mutex));

      // Loops through all bytes from buffer and increment by one in the index that indicate the Ascii value.
      for (int i = 0; i < psBuffer->bytes_in_buffer; i++) {
         cHolder = psBuffer->buffer[i];
         if (cHolder > 31 && cHolder < 127) {
            psBuffer->count[cHolder - 32]++;
         }
      }

      // Check if any of the words occur.
      CheckBufferForWords(psBuffer->psWords, psBuffer);

      // All bytes used in buffer while reading -> not read whole file.
      if (psBuffer->bytes_in_buffer == BUFFER_SIZE) {
         // Reset buffer, all bytes read.
         psBuffer->bytes_in_buffer = 0;
         // Signal that all bytes are read and ready to be used.
         sem_post(&(psBuffer->sem_empty));
         // Unlock thread.
         pthread_mutex_unlock(&(psBuffer->mutex));

         // Not all bytes used in buffer while reading -> last part of file is read -> break and cleanup.
      } else if (psBuffer->bytes_in_buffer < BUFFER_SIZE) {
         // Reset buffer, all bytes read.
         psBuffer->bytes_in_buffer = 0;
         // Signal that all bytes are read and ready to be used.
         sem_post(&(psBuffer->sem_empty));
         // Unlock mutex.
         pthread_mutex_unlock(&(psBuffer->mutex));
         break;
      }
   }
   // Prints all values from count.
   for (int i = 0; i < BYTE_RANGE; i++) {

      printf("%c: %d\n", i + 32, psBuffer->count[i]);
   }

   printf("and: %d\n", psBuffer->psWords->iaOccurrenceOfWords[0]);
   printf("at: %d\n", psBuffer->psWords->iaOccurrenceOfWords[1]);
   printf("it: %d\n", psBuffer->psWords->iaOccurrenceOfWords[2]);
   printf("my: %d\n", psBuffer->psWords->iaOccurrenceOfWords[3]);
   printf("Hamlet: %d\n", psBuffer->psWords->iaOccurrenceOfWords[4]);
   for (int i = 0; i < 5; i++) {

   }
   // Exit thread, returning nothing to the join.
   pthread_exit(NULL);
}











