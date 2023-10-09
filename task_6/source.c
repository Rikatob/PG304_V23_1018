
#include "pgdbglog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 512

void ReplaceSpace(char *szInputBuffer, char *szOutputBuffer, FILE *fpInput);

void FindUnIntNames(const char *szBuffer, char *szNameHolder);

bool ExistsInVarNames(char *szNameHolder, char cCurrent, char cNext);

void ReplaceHungarian(const char *szInputBuffer, char *szOutputBuffer, char *szNameHolder);

void BufferSwapper(char *szInputBuffer, char *szOutputBuffer);

bool IsWhileLoop(const char *szInputBuffer, int iIndex, unsigned long ulBufferLength);

void FindConditionOperands(const char *szBuffer, char *cpFirstOpperand, char *cpSecondOperand, int iIndex,
                           unsigned long ulBufferLength);

void HardCodeForLoop(const char *szInputBuffer, char *szOutputBuffer, char cOperandValue, char cConditionsFirstOperand,
                     char cConditionsSecondOperand, int *iIndex, int *i);

int main(int iArgC, char *apszArgV[]) {

   char szInputBuffer[MAX_BUFFER_SIZE] = {0};
   char szOutputBuffer[MAX_BUFFER_SIZE] = {0};
   char szNameHolder[100] = {0};
   char cOperandValue = {0};
   char cConditionsFirstOperand;
   char cConditionsSecondOperand;
   int iIndex = 0;

   FILE *fpInput;
   FILE *fpOutput;

   PgDbgInitialize();

   fpInput = fopen("testfil.c", "r");
   fpOutput = fopen("output.c", "w");

   if (fpInput == NULL || fpOutput == NULL) {
      pgerror("Could not open file.");
      exit(1);
   }


   //////////////////////////////////////////////////
   // First round of beautifying(Space)            //
   // After this step i use arrays instead of file.//
   //////////////////////////////////////////////////
   ReplaceSpace(szInputBuffer, szOutputBuffer, fpInput);
   fclose(fpInput);

   ///////////////////////////////////////////////
   // Second round of beautifying(While to for) //
   ///////////////////////////////////////////////

   // Swap buffers.
   BufferSwapper(szInputBuffer, szOutputBuffer);
   unsigned long ulBufferLength = strlen(szInputBuffer);

   for (int i = 0; i < ulBufferLength; i++) {
      // I is a digit.
      if (isdigit(szInputBuffer[i]) > 0) {
         // Check if next is while loop.
         if (IsWhileLoop(szInputBuffer, i, ulBufferLength)) {
            // Set value of what index loop should start on. ( int i = 0, int i = 5 etc)
            cOperandValue = szInputBuffer[i];
            // Find the two operands inside the condition ex, 'a' and 'b' when 'while(a < b)'.
            FindConditionOperands(szInputBuffer, &cConditionsFirstOperand, &cConditionsSecondOperand, i,
                                  ulBufferLength);
            // Sorry to say, but hardcode the for-loop format with the gathered values.
            HardCodeForLoop(szInputBuffer, szOutputBuffer, cOperandValue, cConditionsFirstOperand,
                            cConditionsSecondOperand, &iIndex, &i);
            i = i + 12;
            continue;
         }
      }
      szOutputBuffer[iIndex] = szInputBuffer[i];
      iIndex++;
   }


   ////////////////////////////////////////////////////
   // Third round of beautifying(Hungarian notation) //
   ////////////////////////////////////////////////////

   // Find occurrence of unsigned int declarations.
   FindUnIntNames(szOutputBuffer, szNameHolder);

   // Swap buffers.
   BufferSwapper(szInputBuffer, szOutputBuffer);

   // Replace all unsigned ints with hungarian notation.
   ReplaceHungarian(szInputBuffer, szOutputBuffer, szNameHolder);


   szOutputBuffer[MAX_BUFFER_SIZE - 1] = '\0';
   fputs(szOutputBuffer, fpOutput);
   fclose(fpOutput);
}

void HardCodeForLoop(const char *szInputBuffer, char *szOutputBuffer, char cOperandValue, char cConditionsFirstOperand,
                     char cConditionsSecondOperand, int *iIndex, int *i) {
   szOutputBuffer[(*iIndex)] = szInputBuffer[(*i)];
   (*iIndex)++;
   (*i)++;
   szOutputBuffer[(*iIndex)] = szInputBuffer[(*i)];
   (*iIndex)++;
   (*i)++;
   szOutputBuffer[(*iIndex)] = szInputBuffer[(*i)];
   (*iIndex)++;
   (*i)++;
   szOutputBuffer[(*iIndex)] = szInputBuffer[(*i)];
   (*iIndex)++;
   (*i)++;
   szOutputBuffer[(*iIndex)] = 'f';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = 'o';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = 'r';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = '(';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = cConditionsFirstOperand;
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = '=';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = cOperandValue;
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ';';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = 'a';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = '<';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = cConditionsSecondOperand;
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ';';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ' ';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = cConditionsFirstOperand;
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = '+';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = '+';
   (*iIndex)++;
   szOutputBuffer[(*iIndex)] = ')';
   (*iIndex)++;
}

void FindConditionOperands(const char *szBuffer, char *cpFirstOpperand, char *cpSecondOperand, int iIndex,
                           unsigned long ulBufferLength) {
   for (int i = iIndex; i < ulBufferLength; i++) {

      if (szBuffer[i] == 40) {
         *cpFirstOpperand = szBuffer[i + 1];
      } else if (szBuffer[i] == 41) {
         *cpSecondOperand = szBuffer[i - 1];
         break;
      }
   }
}

bool IsWhileLoop(const char *szInputBuffer, int iIndex, unsigned long ulBufferLength) {
   bool bReturnValue = false;
   if (szInputBuffer[iIndex + 1] == 59) {
      if (szInputBuffer[iIndex + 2] == '\n') {
         if (szInputBuffer[iIndex + 3] == 9) {
            if (szInputBuffer[iIndex + 4] == 'w') {
               if (szInputBuffer[iIndex + 5] == 'h') {
                  if (szInputBuffer[iIndex + 6] == 'i') {
                     bReturnValue = true;
                  }
               }
            }
         }
      }
   }

   return bReturnValue;
}

// Clean input buffer -> Swap buffers -> Clean output buffer.
void BufferSwapper(char *szInputBuffer, char *szOutputBuffer) {
   memset(szInputBuffer, 0, MAX_BUFFER_SIZE);
   memcpy(szInputBuffer, szOutputBuffer, MAX_BUFFER_SIZE);
   memset(szOutputBuffer, 0, MAX_BUFFER_SIZE);
}


void ReplaceHungarian(const char *szInputBuffer, char *szOutputBuffer, char *szNameHolder) {
   int iIndex = 0;
   unsigned long ulBufferLength = strlen(szInputBuffer);
   bool bIsName = false;

   for (int i = 0; i < ulBufferLength; i++) {

      if (szInputBuffer[i] == 32 || isalpha(szInputBuffer[i]) == 0) {

         bIsName = ExistsInVarNames(szNameHolder, szInputBuffer[i + 1], szInputBuffer[i + 2]);

         if (bIsName) {
            szOutputBuffer[iIndex] = szInputBuffer[i];
            szOutputBuffer[iIndex + 1] = 'u';
            szOutputBuffer[iIndex + 2] = 'i';
            szOutputBuffer[iIndex + 3] = (char) toupper(szInputBuffer[i + 1]);
            bIsName = false;
            i = i + 1;
            iIndex = iIndex + 4;
            continue;
         }
      }
      szOutputBuffer[iIndex] = szInputBuffer[i];
      iIndex++;
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// * Check if cCurrent argument exists in szNameHolder (array containing name of variables). //
///////////////////////////////////////////////////////////////////////////////////////////////
bool ExistsInVarNames(char *szNameHolder, char cCurrent, char cNext) {
   unsigned long ulNameHolderLength = strlen(szNameHolder);
   bool bReturnValue = false;

   for (int i = 0; i < ulNameHolderLength; i++) {
      if (cCurrent == szNameHolder[i] && cNext == 32) {
         bReturnValue = true;
         break;
      }
      if (cCurrent == szNameHolder[i] && isalpha(cNext) == 0) {
         bReturnValue = true;
         break;
      }
   }
   return bReturnValue;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// *Search the buffer for unsigned int declarations and store the variable name in szNameHolder.//
//////////////////////////////////////////////////////////////////////////////////////////////////
void FindUnIntNames(const char *szBuffer, char *szNameHolder) {
   int iIndex = 0;
   unsigned long ulBufferLength = strlen(szBuffer);
   char szUnsIntId[13] = "unsigned int\0";


   for (int i = 0; i < ulBufferLength; i++) {
      // No room for more UINT variable names.
      if (iIndex >= 98) {
         break;
      }

      if (szBuffer[i] == 'u') {
         if (i <= ulBufferLength - 13) {
            if (strncmp(szBuffer + i, szUnsIntId, 12) == 0) {
               szNameHolder[iIndex] = szBuffer[i + 13];
               i = i + 13;
               iIndex++;
            }
         }
      }
   }
   // Ensure zero terminator.
   szNameHolder[99] = '\0';
}


////////////////////////////////////////////////////////////////
// * Loops through the file and replace all 3xSpace with tab. //
// * Store the step 1 beautified code in output buffer.       //
////////////////////////////////////////////////////////////////
void ReplaceSpace(char *szInputBuffer, char *szOutputBuffer, FILE *fpInput) {
   unsigned long ulInputLength;
   int iIndex = 0;
   int iReplacementCounter = 0;

   pgdebug("Started replacing spaces");
   while (fgets(szInputBuffer, MAX_BUFFER_SIZE - 1, fpInput) != NULL) {

      ulInputLength = strlen(szInputBuffer);

      for (int i = 0; i < ulInputLength; i++) {

         if (szInputBuffer[i] == 32) {

            if (i <= ulInputLength - 3) {

               if (szInputBuffer[i + 1] == 32 && szInputBuffer[i + 2] == 32) {
                  szOutputBuffer[iIndex] = 9;
                  iIndex++;
                  i = i + 2;
                  iReplacementCounter++;
                  continue;
               }
            }
         }
         szOutputBuffer[iIndex] = szInputBuffer[i];
         iIndex++;
      }
   }

   // Ensure zeroterminator.
   szOutputBuffer[MAX_BUFFER_SIZE - 1] = '\0';
   pgdebug("Replaced with tab %d places.", iReplacementCounter);
}

