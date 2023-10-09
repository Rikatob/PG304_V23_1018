#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgdbglog.h"
#include "source.h"
#include "server.h"
#include "client.h"
#include <arpa/inet.h>


int main(int iArgC, char *apszArgV[]) {
   struct sockaddr_in saAddr = {0}; // Bind
   struct sockaddr_in saConClient = {0}; // Accept
   int sockFd, sockConnFd = 0, iPort = -1, addrLen = sizeof(saAddr);
   int iReturnCode = 0;
   char buffer[MAX_BUFFER_SIZE];
   bool bRunServer = false;
   bool bRunClient = false;
   PgDbgInitialize();


   // User entered to few arguments when executing th program.
   if (iArgC < 4) {
      pgerror("Tried to run program with to few arguments");
      printf("Use 'source -listen -port <portNumber>' to start server\n");
      printf("Use 'source -server <ip-adress> -port <portNumber>' to start client\n");
      return 1;
   }

   // Check if arguments indicate to start as server.
   if (strncasecmp(apszArgV[1], "-listen", 7) == 0) {
      if (strncasecmp(apszArgV[2], "-port", 5) != 0) {
         printf("Wrong format on arguments trying to start server!\n");
         return 1;
      } else {
         printf("Server is started\n");
         bRunServer = true;
      }

      // Check if arguments indicate to start as client.
   } else if (strncasecmp(apszArgV[1], "-server", 7) == 0) {
      if (strncasecmp(apszArgV[3], "-port", 5) != 0) {
         printf("Wrong format on arguments trying to start client!\n");
         return 1;
      } else {
         printf("Client is started\n");
         bRunClient = true;
      }
   } else {
      printf("Invalid input!\n");
      return 1;
   }

   // Server successfully chosen to execute.
   if (bRunServer) {

      // Check port number entered.
      if (CheckIfValidPortNumber(apszArgV[3])) {
         iPort = atoi(apszArgV[3]);
         pgdebug("Portnumber [%d] accepted.", iPort);
      } else {
         printf("Port number not accepted.\n");
         pgerror("Portnumber [%d] is not valid.", iPort);
         return 1;
      }

      // Create and verify socket.
      sockFd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockFd < 0) {
         pgerror("Socket failed to create [errno=%i]", errno);
      } else {
         pgdebug("Socket successfully created");
      }

      // Assigning IP and Port
      saAddr.sin_family = AF_INET;
      saAddr.sin_port = htons(iPort);
      saAddr.sin_addr.s_addr = htonl(0x7F000001);

      // Bind socket to IP and verify if succeeded.
      if (bind(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
         pgerror("Socket bind failed with %i", errno);
         return 1;
      } else {
         pgdebug("Socket successfully bind to %d", saAddr.sin_addr);
      }
      // Listen for client to connect.
      if (listen(sockFd, 5) < 0) {// how many can connect to the socket.
         pgerror("Server listen failed");
         return 1;
      } else {
         pgdebug("Server listening...");
      }

      /////////////////
      /// -> DIALOG ///
      /////////////////
      iReturnCode = ERROR;

      // While connection is not lost.
      while (iReturnCode != DONE) {

         // Initiate socket for client to connect.
         iReturnCode = InitConnectionSocket(sockFd, &sockConnFd, saConClient, addrLen);
         // Simple check if initialization of socket failed.
         if (iReturnCode == ERROR) {
            return 1;
         }
         // Start chat.
         iReturnCode = StartServerChat(buffer, sockConnFd);
         // Simple check if something in the chat failed.
         if (iReturnCode == ERROR) {
            close(sockConnFd);
            sockConnFd = -1;
            return 1;
         }
         // Clean up.
         close(sockConnFd);
         sockConnFd = -1;
      }

      // Client successfully chosen to execute.
   } else if (bRunClient) {

      // Check port number entered.
      if (CheckIfValidPortNumber(apszArgV[4])) {
         iPort = atoi(apszArgV[4]);
         pgdebug("Portnumber [%d] accepted.", iPort);
      } else {
         printf("Port number not accepted.\n");
         pgerror("Portnumber [%d] is not valid.", iPort);
         return 1;
      }

      // Create and verify socket.
      sockFd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockFd < 0) {
         pgerror("Socket failed to create [errno=%i]", errno);
      } else {
         pgdebug("Socket successfully created");
      }

      // Assigning IP and Port
      saAddr.sin_family = AF_INET;
      saAddr.sin_port = htons(iPort);  // Port should be short.
      iReturnCode = inet_pton(AF_INET, apszArgV[2], &saAddr.sin_addr.s_addr);
      if (iReturnCode == 0) {
         pgerror("Invalid ip address used, program terminating..");
         return 1;
      }

      // Connect to server socket.
      if (connect(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
         pgerror("Connection to server failed with [errno=%i]", errno);
      } else {
         pgdebug("Successfully connected to server [%d]", saAddr.sin_addr);
      }

      /////////////////
      /// -> DIALOG ///
      /////////////////
      // Start chat.
      iReturnCode = StartClientChat(buffer, sockFd);
      // Simple check if chat failed.
      if (iReturnCode == ERROR) {
         return 1;
      }

      // Error neither server nor client was successfully chosen.
   } else {
      pgerror("Something went wrong!");
      return 1;
   }

   // Cleanup.
   close(sockFd);
   sockFd = -1;
   return 0;
}


///////////////////////////////////
// * Check for portnumber input. //
// * Returns                     //
//   -> 0 (false) if not valid.  //
//   -> 1 (true) if valid.       //
///////////////////////////////////
int CheckIfValidPortNumber(char *portNmbr) {

   for (int i = 0; i < strlen(portNmbr); i++) {
      if (!isdigit(portNmbr[i]) || portNmbr[i] == -1) {
         return 0;
      }
   }

   return 1;
}


///////////////////////////////////////////////////////////////
// * Store input stream from socket in buffer in proper way. //
// * Return                                                  //
//   -> ERROR somthing went wrong.                           //
//   -> DONE FIN package received , close connection.        //
//   -> OK message successfully received, keep connection up.//
///////////////////////////////////////////////////////////////
int ReceiveMessage(char *szBuffer, int sockConnFd) {
   memset(szBuffer, 0, MAX_BUFFER_SIZE);
   // MSG_FIN flag makes recv return 0 if fin package is received.
   int receiveValue = (int) recv(sockConnFd, szBuffer, MAX_BUFFER_SIZE - 1, MSG_FIN);

   if (receiveValue < 0) {
      pgerror("Receive failed with [errno = %i]", errno);
      return ERROR;
   } else if (receiveValue == 0) {
      pgdebug("User have terminated the connection.");
      return DONE;
   }
   pgdebug("Message successfully received");
   return OK;
}


/////////////////////////////////////////////////////////
// -> Gets input from stdin and store it in the szBuffer.//
/////////////////////////////////////////////////////////
void GetInput(char *szBuffer) {
   fgets(szBuffer, MAX_BUFFER_SIZE - 1, stdin);
   // Remove the newline from the input.
   szBuffer[strlen(szBuffer) - 1] = 0;
   // ensure zero terminator.
   szBuffer[MAX_BUFFER_SIZE - 1] = '\0';
}
