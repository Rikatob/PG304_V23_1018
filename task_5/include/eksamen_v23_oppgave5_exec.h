

#ifndef _TASK_5_EKSAMEN_V23_OPPGAVE5_EXEC_H_
#define _TASK_5_EKSAMEN_V23_OPPGAVE5_EXEC_H_

#define MAX_LINES 10


typedef struct _RESPONSE {
   int iSize;                  // Header
   char *apszLines[MAX_LINES]; // Data
} RESPONSE;


char* ExecuteCommand(const char *pcszCommand);

#endif
