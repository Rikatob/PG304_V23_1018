#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "client.h"
#include "pgdbglog.h"
#include "source.h"
#include "eksamen_v23_oppgave5_exec.h"


/////////////////////////////////////////////////////////////////////////////////////////
// * Desc                                                                              //
//   -> Wait for command from server.                                                  //
//   -> Execute command.                                                               //
//   -> Send command response to server.                                               //
//   -> Handles 'ctrl + c'                                                             //
// * Returns                                                                           //
//  -> ERROR if something went wrong.                                                  //
//  -> DONE if chat should be terminated.                                              //
/////////////////////////////////////////////////////////////////////////////////////////
int StartClientChat(char *szBuffer, int sockFd) {
   char *pszReturnString = NULL;
   int iReturnCode = ERROR;

   while (1) {
      iReturnCode = ReceiveMessage(szBuffer, sockFd);
      if (iReturnCode != ERROR) {

         // Server has terminated the connection.
         if (iReturnCode == DONE || strncasecmp(szBuffer, "quit", 4) == 0) {
            pgdebug("Client quitting, server terminated connection.")
            printf("Server terminated connection..");
            break;

         } else {
            printf("\tFrom server: %s\n", szBuffer);

            // Execute command from szBuffer and store response in return string.
            pszReturnString = ExecuteCommand(szBuffer);
            pgdebug("From commando:%s ", pszReturnString);

            // Send the response from command.
            send(sockFd, pszReturnString, strlen(pszReturnString), MSG_NOSIGNAL);

            // Cleanup
            free(pszReturnString);
         }
      }
   }

   return iReturnCode;
}