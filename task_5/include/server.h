

#ifndef _TASK_5_SERVER_H_
#define _TASK_5_SERVER_H_

///////////////////////////////////FUNCTION PROTOTYPES///////////////////////////////////

int StartServerChat(char *szBuffer, int sockConnFd);

int InitConnectionSocket(int ipSockFd, int *ipSockConnFd, struct sockaddr_in saConClient, int addrLen);

#endif
