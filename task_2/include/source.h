
#ifndef TASK_2_SOURCE_H
#define TASK_2_SOURCE_H

#include <stdbool.h>

#define BUFFER_SIZE 200

typedef struct _NUMBER_METADATA {
   int iIndex;
   int iNumber;
   bool bIsFibonacci;
   bool bIsPrimeNumber;
   bool bIsSquareNumber;
   bool bIsCubeNumber;
   bool bIsPerfectNumber;
   bool bIsAbundantNumber;
   bool bIsDeficientNumber;
   bool bIsOddNumber;
} NUMBER_METADATA;

///////////////////////////////////// FUNCTION PROTOTYPES /////////////////////////////////////

void CheckNumber(int iNumber, int *iIndex, NUMBER_METADATA *psNumberMetadata);

#endif //TASK_2_SOURCE_H
