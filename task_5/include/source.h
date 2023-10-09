
#ifndef _TASK_5_SOURCE_H_
#define _TASK_5_SOURCE_H_

#define MAX_BUFFER_SIZE 512
#define OK 0
#define ERROR 1
#define DONE 2

///////////////////////////////////FUNCTION PROTOTYPES///////////////////////////////////
int CheckIfValidPortNumber(char *portNmbr);
int ReceiveMessage(char *szBuffer, int sockConnFd);
void GetInput(char *szBuffer);

#endif