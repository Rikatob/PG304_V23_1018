#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pgdbglog.h"
#include "source.h"
#include "eksamen_v23_oppgave2_abun.h"
#include "eksamen_v23_oppgave2_cube.h"
#include "eksamen_v23_oppgave2_def.h"
#include "eksamen_v23_oppgave2_fib.h"
#include "eksamen_v23_oppgave2_kvad.h"
#include "eksamen_v23_oppgave2_odd.h"
#include "eksamen_v23_oppgave2_perf.h"
#include "eksamen_v23_oppgave2_prim.h"


int main(int iArgC, char *apszArgV[]) {
   int iNumber = 0;
   int iNumberIndex = 1; // initiate with 1 to indicate the first number in the input file.
   char *szInputFileName = "eksamen_v23_oppgave2.txt";
   char *szOutputFileName = "outputBinary.bin";
   char buffer[BUFFER_SIZE] = {0}; // initiate with zero terminators.
   NUMBER_METADATA sNumberMetadata = {0};
   FILE *pfNumbers = NULL;
   FILE *pfOutput = NULL;

   // Init the dbg logger.
   PgDbgInitialize();

   // Open input file.
   pfNumbers = fopen(szInputFileName, "r");

   // Open output file.
   pfOutput = fopen(szOutputFileName, "wb");

   // Check input file and manage thereafter.
   if (pfNumbers == NULL) {
      pgerror("There was an error opening %s!", szInputFileName);
      return 1;
   }
   pgdebug("%s was successfully opened.", szInputFileName);

   // Check output file and manage thereafter.
   if (pfOutput == NULL) {
      pgerror("There was an error opening %s!", szOutputFileName);
      return 1;
   }
   pgdebug("%s was successfully opened.", szOutputFileName);


   // Reads file until it reach newline -> store the sentence in buffer -> and repeats until EOF.
   // BUFFER_SIZE - 1 to ensure there is a zero terminator (as buffer is initiated with all zero terminator).
   while (fgets(buffer, BUFFER_SIZE - 1, pfNumbers)) {

      iNumber = atoi(buffer); // will return MAX_INT or 0 if failed.

      // Line in file is valued to equal or bigger than max size of integer and is defined as suspicious behavior.
      // This will log an error and terminate the program.
      if (iNumber == INT_MAX || iNumber <= 0) {
         pgerror("Suspicious behavior in %d at index %d, Program terminated.", iNumber, iNumberIndex);
         // Close files and remove dangling references.
         fclose(pfNumbers);
         pfNumbers = NULL;
         fclose(pfOutput);
         pfOutput = NULL;
         return 1;
      }
      CheckNumber(iNumber, &iNumberIndex, &sNumberMetadata);
      fwrite(&sNumberMetadata, sizeof(NUMBER_METADATA), 1, pfOutput);
   }

   // Close files and remove dangling references.
   fclose(pfNumbers);
   pfNumbers = NULL;
   fclose(pfOutput);
   pfOutput = NULL;

   return 0;
}


////////////////////////////////////////////////////////////////////////////////
// * Args                                                                     //
//  -> The number to check.                                                   //
//  -> The index the number is.                                               //
//  -> A pointer to the struct to store the values in.                        //
// * Use the different functions to define if the number is perfect, odd etc  //
//    and store that information in the struct.                               //
// * Increment the index for each number checked.                             //
////////////////////////////////////////////////////////////////////////////////
void CheckNumber(int iNumber, int *iIndex, NUMBER_METADATA *psNumberMetadata) {
   int iCurrIndex = *iIndex;
   psNumberMetadata->iIndex = iCurrIndex;
   psNumberMetadata->iNumber = iNumber;
   psNumberMetadata->bIsAbundantNumber = isAbundantNumber(iNumber);
   psNumberMetadata->bIsCubeNumber = isCubeNumber(iNumber);
   psNumberMetadata->bIsDeficientNumber = isDeficientNumber(iNumber);
   psNumberMetadata->bIsFibonacci = isFibonacci(iNumber);
   psNumberMetadata->bIsSquareNumber = isSquareNumber(iNumber);
   psNumberMetadata->bIsOddNumber = isOdd(iNumber);
   psNumberMetadata->bIsPerfectNumber = isPerfectNumber(iNumber);
   psNumberMetadata->bIsPrimeNumber = isPrime(iNumber);

   pgdebug("Number %d at Index %d successfully checked", iNumber, iCurrIndex);

   // Add one to the index for each number checked.
   iCurrIndex++;
   *iIndex = iCurrIndex;
}