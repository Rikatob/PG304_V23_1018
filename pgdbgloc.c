#include "pgdbglog.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

////////////////////////NOTES/////////////////////////////////////////////
// * The file is not closed because we do                               //
// not want to open the file every time PgDbgLogger is called.          //
// * I needed to "dummy down" the code because of 3rd party library     //
// Solved this with hardcoding the config values in parsingXmlFile.     //
// Horrible way of doing it, but i needed to focus the time on examprep.//
// SORRY!                                                               //
//////////////////////////////////////////////////////////////////////////

void PgDbgLogger(unsigned long ulErrorType, int iLine, const char *szFile, const char *pszFormat, ...) {

   static int iCallCounter = 1;
   static bool sbHasBeenConfigurated = false;
   char szOutputString[256] = {0};
   // If ulErrorType is 1 -> Error, if not -> Debug.
   const char *pszType = (ulErrorType == 1) ? "Error" : "Debug";
   char *pathDebugLog;
   char szFileName[256] = {0};
   char xmlProperties[AMOUNT_OF_XML_PROPS][LENGTH_OF_PROP_VALUE] = {0};
   time_t tTimeAndDate = 0;
   va_list vaArgumentPointer;
   static FILE *fLogFile = NULL;

   // Configurating the properties from config.xml if it has not already been configured.
   // if more properties are added to config.xml
   // -> change the xmlProperties-array to hold more properties. [2][80] for 3 prop, [3][80] for 4 and so on.
   if (!sbHasBeenConfigurated) {
      ParsingXmlFile(xmlProperties);
      pathDebugLog = xmlProperties[1];
      sbHasBeenConfigurated = true;
   }
   ////////////////////////////////////////////////////////////////////
   // Check if the file is opened.									  			//
   // if not it will open the file.								  				//
   // to avoid opening the file again every time function is called  //
   // and overwriting existing values for each time. 				  		//
   ////////////////////////////////////////////////////////////////////
   if (fLogFile == NULL) {
      tTimeAndDate = time(NULL);
      snprintf(szFileName, 256 - 1, "%sdebug%lu.txt", pathDebugLog, tTimeAndDate);
      fLogFile = fopen(szFileName, "w"); // "w" will be truncated on open.
   }

   ///////////////////////////////////////////////
   // check if the filePointer is null				//
   // which can be if the file does not exist. 	//
   ///////////////////////////////////////////////
   if (fLogFile == NULL) {
      printf("%sdebug.txt file failed to open.", pathDebugLog);

   } else {
      va_start(vaArgumentPointer, pszFormat);
      vsnprintf(szOutputString, 256 - 1, pszFormat, vaArgumentPointer);
      va_end(vaArgumentPointer);
      fprintf(fLogFile, "%04i: Type=%s Line=%d File=%s {%s}\n", iCallCounter, pszType, iLine, szFile, szOutputString);
      fflush(fLogFile);
      iCallCounter++;
   }

}


void PgDbgInitialize() {
   char xmlProperties[AMOUNT_OF_XML_PROPS][LENGTH_OF_PROP_VALUE];
   // Parsing xml and setting the properties from config.
   ParsingXmlFile(xmlProperties);
   glob_bTraceEnabled = atoi(xmlProperties[0]);

}


void ParsingXmlFile( char xmlProperties[][LENGTH_OF_PROP_VALUE]) {

   xmlProperties[0][0] = '1';
   xmlProperties[0][1] = '\0';

   xmlProperties[1][0] = '.';
   xmlProperties[1][1] = '/';
   xmlProperties[1][2] = 'd';
   xmlProperties[1][3] = 'e';
   xmlProperties[1][4] = 'b';
   xmlProperties[1][5] = 'u';
   xmlProperties[1][6] = 'g';
   xmlProperties[1][7] = 'L';
   xmlProperties[1][8] = 'o';
   xmlProperties[1][9] = 'g';
   xmlProperties[1][10] = 's';
   xmlProperties[1][11] = '/';
   xmlProperties[1][12] = '\0';
}

