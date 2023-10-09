#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"
#include "pgdbglog.h"


// AUTHOR : BENGT ØSTBY.
// MODIFIED BY: 1018.


/* The popen() function is a standard C library function that allows a program to open a pipe to 
   a process for reading or writing. The name popen stands for "process open". The function takes 
   two arguments: a string specifying the command to execute, and a string specifying the mode 
   in which the pipe should be opened. The mode string should be either "r" (to open the pipe for 
   reading) or "w" (to open the pipe for writing). The function returns a FILE pointer that can be 
   used to read from or write to the pipe.

   The below function takes a command as argument, executes that command by calling popen, and 
   returns as a string the output of that command, as would have been printed to the terminal.*/

char *ExecuteCommand(const char *pcszCommand) {
   unsigned long iReturnStringLength = 0;
   FILE *fp = NULL;
   char *pszReturnString = malloc(MAX_BUFFER_SIZE);
   char *szTemp = malloc(MAX_BUFFER_SIZE);

   // Check if allocating was successfully done.
   if (pszReturnString == NULL || szTemp == NULL) {
      return NULL;
   }
   // Clean sheets.
   memset(pszReturnString, 0, MAX_BUFFER_SIZE);
   memset(szTemp, 0, MAX_BUFFER_SIZE);

   fp = popen(pcszCommand, "r");
   if (fp == NULL) {
      sprintf(pszReturnString, "Error: Failed to execute command");
   } else {

      while (fgets(szTemp, MAX_BUFFER_SIZE - 1, fp) != NULL) {
         iReturnStringLength = strlen(pszReturnString);
         // Check if combined length of temp and returnstring is bigger than allowed.
         if (iReturnStringLength + strlen(szTemp) >= MAX_BUFFER_SIZE - 1) {
            pgdebug("to long respons, respons is cut.");
            break;
         }
         // Concat the buffer to the end of the returnstring.
         strncat(pszReturnString, szTemp, MAX_BUFFER_SIZE);
      }
      // If no response is given from the client its assumed it is a command with no return value like 'mkdir'.
      if (iReturnStringLength == 0) {
         strncpy(pszReturnString, "Command executed!\n", 18);
      }

      // Ensure zero termination.
      pszReturnString[MAX_BUFFER_SIZE - 1] = '\0';

      // Cleanup.
      free(szTemp);
      pclose(fp);
   }
   return pszReturnString;
}

/* IMPORTANT NOTE: This function has alloced a string by malloc, it must be freed BY THE CALLER...*/
