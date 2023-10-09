#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "pgdbglog.h"
#include "server.h"
#include "source.h"


/////////////////////////////////////////////////////////////////////////////////////////
// * Desc                                                                              //
//   -> Get input from user                                                            //
//   -> Send input.                                                                    //
//   -> Get response from client.                                                      //
//   -> Server can send 'quit' to terminate connection and close server and client     //
//   -> Handles 'ctrl + c'                                                             //
// * Returns                                                                           //
//  -> ERROR if something went wrong.                                                  //
//  -> DONE if chat should be terminated.                                              //
//  -> Can implement OK if server should keep listening for another client to connect. //
/////////////////////////////////////////////////////////////////////////////////////////
int StartServerChat(char *szBuffer, int sockConnFd) {
   int iReturnCode = ERROR;
   long lSendReturnCode = -1;
   printf("Client connected...\n");
   while (1) {
      // Clean sheets.
      memset(szBuffer, 0, MAX_BUFFER_SIZE);

      printf("\tTo client : ");
      // Get input from server and put in szBuffer.
      GetInput(szBuffer);
      // Send szBuffer to client.
      lSendReturnCode = send(sockConnFd, szBuffer, strlen(szBuffer), MSG_NOSIGNAL);

      if(lSendReturnCode < 0){
         pgerror("Sending failed with errno[%i]",errno);
         iReturnCode = ERROR;
         break;
      }

      // If the message sent contains "quit" then server terminated the chat.
      if (strncasecmp(szBuffer, "quit", 4) == 0) {
         pgdebug("Server quitting and terminated the connection.");
         printf("\nServer terminated..\n");
         iReturnCode = DONE;
         break;
      }

      // Receive message from client and store szBuffer.
      iReturnCode = ReceiveMessage(szBuffer, sockConnFd);
      if (iReturnCode != ERROR)
      {
         printf("From client : %s\n", szBuffer);
         // Client forced disconnection.
         if (iReturnCode == DONE) {
            pgdebug("Client terminated connection.");
            printf("Client forced disconnection..\n");
            break;
         }
      }

   }

   return iReturnCode;
}

int InitConnectionSocket(int ipSockFd, int *ipSockConnFd, struct sockaddr_in saConClient, int addrLen) {
   *ipSockConnFd = accept(ipSockFd, (struct sockaddr *) &saConClient, (socklen_t *) &addrLen);
   if (ipSockConnFd < 0) {
      pgerror("Accept failed with %i", errno);
      return ERROR;
   }
   pgdebug("Client on [%d] accepted by server", saConClient.sin_addr);

   return OK;
}