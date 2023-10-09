#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "pgdbglog.h"
#include "flightDeparture.h"
#include "source.h"


int main(int iArgC, char *apszArgV[]) {

   int iReturnCode = OK;
   int iDebugChoiceCount = 0;
   int iTmpNumber = 0;
   char cUserChoice = 0;
   char szInputBuffer[INPUT_BUFFER_SIZE] = {0};
   unsigned long ulInputLength = 0;
   bool bInvalidInput = true;
   FLIGHT_DEPARTURES sFlightDepartures = {0};
   RESERVATION *pTmpReservation;
   DEPARTURE *pTmpDeparture;


   // Init debug logger.
   PgDbgInitialize();

   // Menu
   do {
      iDebugChoiceCount++;
      PrintMenu();
      cUserChoice = getchar(); // getchar is safe to use , only takes one byte.
      FlushStdinBuffer();
      pgdebug("Users %i choice was [%c]", iDebugChoiceCount, cUserChoice);

      switch (cUserChoice) {

         case '1':
            iReturnCode = ERROR;

            printf("You have selected to add a departure\n");
            printf("Enter the departure details\n");

            iReturnCode = AddFlightDeparture(&sFlightDepartures, szInputBuffer);

            if (iReturnCode == ERROR) {
               pgerror("Program terminated");
               return 1;
            }
            break;

         case '2':
            iReturnCode = ERROR;
            printf("Add Reservation selected\n");
            printf("Enter the index on the departure you want create a reservation\n ");


            iReturnCode = GetDepartureOnIndex(&sFlightDepartures, szInputBuffer, &pTmpDeparture);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong getting departure");
               break;
            }

            iReturnCode = AddFlightReservation(&(pTmpDeparture->sReservations), szInputBuffer, pTmpDeparture->iSeats);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong adding reservation.");
               break;
            }
            break;

         case '3':

            iReturnCode = ERROR;

            printf("Get info on departure by index selected\n");

            ////////////////////
            // Find departure.//
            iReturnCode = GetDepartureOnIndex(&sFlightDepartures, szInputBuffer, &pTmpDeparture);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong getting departure");
               break;
            }

            /////////////////////
            // Print departure.//
            iReturnCode = PrintDeparture(pTmpDeparture);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong printing departure");
               break;
            }

            break;

         case '4':
            printf("Departure on TFD selected\n");
            iReturnCode = ERROR;
            printf("Enter time for departure [0001 - 2400]\n");

            iTmpNumber = GetIntegerInput(szInputBuffer);


            // Not valid time.
            if (iTmpNumber > 2400) {
               while (iTmpNumber > 2400) {
                  printf("Warning, Kindly enter time in format 0001 - 2400\n");
                  iTmpNumber = GetIntegerInput(szInputBuffer);
               }
            }
            //////////////////////
            // * Find departure //
            iReturnCode = FindDepartureOnTFD(&sFlightDepartures, iTmpNumber, &iTmpNumber);

            ////////////////////
            // * print result.//
            ////////////////////
            if (iReturnCode == ERROR) {
               printf("No departure on that time.\n");
            } else {
               printf("Index on the departure traveling at time entered is [%d]", iTmpNumber);
            }

            break;

         case '5':
            printf("Delete departure selected\n");
            printf("Enter the index on the departure you want to delete\n ");

            iReturnCode = ERROR;
            ulInputLength = 0;
            bInvalidInput = true;

            ////////////////////
            // Find departure.//
            iReturnCode = GetDepartureOnIndex(&sFlightDepartures, szInputBuffer, &pTmpDeparture);
            if (iReturnCode == ERROR) {
               pgerror("Something went wrong getting departure");
               break;
            }

            ////////////////////////
            // Delete reservation.//
            iReturnCode = RemoveDeparture(&sFlightDepartures, pTmpDeparture->szFlightID);
            if (iReturnCode == ERROR) {
               pgerror("Something went wrong deleting departure");
               break;
            }

            printf("**Departure on successfully deleted**\n");
            break;

         case '6':
            printf("Delete reservation selected\n");
            printf("Enter the index on the departure you want delete a reservation\n ");

            iReturnCode = ERROR;
            ulInputLength = 0;
            bInvalidInput = true;

            ////////////////////
            // Find departure.//
            iReturnCode = GetDepartureOnIndex(&sFlightDepartures, szInputBuffer, &pTmpDeparture);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong getting departure");
               break;
            }
            printf("**Departure found**\n");


            //////////////////////
            // Find reservation.//
            printf("Enter the name on the reservation\n");

            // Verify name input.
            while (bInvalidInput) {
               ClearInputBuffer(szInputBuffer);
               printf("Enter name (MAX = [%d]\n", NAME_MAX_LENGTH);
               GetInput(szInputBuffer);
               ulInputLength = strlen(szInputBuffer);

               if (ulInputLength <= NAME_MAX_LENGTH) {
                  bInvalidInput = false;
               } else {
                  printf("Name length not accepted!\n");
               }
            }

            iReturnCode = FindReservationOnName(&(pTmpDeparture->sReservations), szInputBuffer, &pTmpReservation);

            if (iReturnCode == ERROR) {
               printf("Reservation not found!\n");
               pgerror("Something went wrong finding reservation");
               break;
            }

            printf("**Reservation found**\n");


            ////////////////////////
            // Delete reservation.//
            iReturnCode = RemoveReservation(&(pTmpDeparture->sReservations), pTmpReservation->szName);
            if (iReturnCode == ERROR) {
               pgerror("Something went wrong deleting reservation");
               break;
            }
            printf("**[%s] successfully deleted**\n", szInputBuffer);
            break;

         case '7':
            printf("Change steat selected\n");

            iReturnCode = ERROR;
            ulInputLength = 0;
            iTmpNumber = 0;
            char szTempName[NAME_MAX_LENGTH + 1] = {0};
            bInvalidInput = true;

            ////////////////////
            // Find departure.//
            iReturnCode = GetDepartureOnIndex(&sFlightDepartures, szInputBuffer, &pTmpDeparture);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong getting departure");
               break;
            }
            printf("**Departure found**\n");


            //////////////////////
            // Find reservation.//
            printf("Enter the name on the reservation\n");

            // Verify name input.
            while (bInvalidInput) {
               ClearInputBuffer(szInputBuffer);
               printf("Enter name (MAX = [%d]\n", NAME_MAX_LENGTH);
               GetInput(szInputBuffer);
               ulInputLength = strlen(szInputBuffer);

               if (ulInputLength <= NAME_MAX_LENGTH) {
                  bInvalidInput = false;
               } else {
                  printf("Name length not accepted!\n");
               }
            }

            iReturnCode = FindReservationOnName(&(pTmpDeparture->sReservations), szInputBuffer, &pTmpReservation);

            if (iReturnCode == ERROR) {
               printf("Reservation not found!\n");
               pgerror("Something went wrong finding reservation");
               break;
            }


            ///////////////////////
            // Change seat number//
            printf("%s currently seated at seat [%d] enter the new seat number.\n", szInputBuffer,
                   pTmpReservation->iSeatNumber);

            // Store name.
            strncpy(szTempName, szInputBuffer, strlen(szInputBuffer));
            szTempName[NAME_MAX_LENGTH] = '\0';


            iReturnCode = SEAT_TAKEN;
            iTmpNumber = GetIntegerInput(szInputBuffer);
            iReturnCode = ChangeSeatNumber(&(pTmpDeparture->sReservations), szTempName, iTmpNumber);

            if (iReturnCode == ERROR) {
               pgerror("Something went wrong changing seat");
               break;

               // Seat taken enter again.
            } else if (iReturnCode == SEAT_TAKEN) {
               while (iReturnCode == SEAT_TAKEN) {
                  printf("Seat is taken choose another.\n");
                  iTmpNumber = GetIntegerInput(szInputBuffer);
                  iReturnCode = ChangeSeatNumber(&(pTmpDeparture->sReservations), pTmpReservation->szName, iTmpNumber);
               }
            }

            printf("**Successfully changed seat**\n");
            break;

         case '8':
            printf("Get departure amount of passenger selected\n");
            iReturnCode = ERROR;
            ulInputLength = 0;
            iTmpNumber = 0;
            bInvalidInput = true;
            pTmpDeparture = sFlightDepartures.pHead;

            ///////////////////////
            // Find reservations.//
            printf("Enter the name you want to check\n");

            // Verify name input.
            while (bInvalidInput) {
               ClearInputBuffer(szInputBuffer);
               printf("Enter name (MAX = [%d]\n", NAME_MAX_LENGTH);
               GetInput(szInputBuffer);
               ulInputLength = strlen(szInputBuffer);

               if (ulInputLength <= NAME_MAX_LENGTH) {
                  bInvalidInput = false;
               } else {
                  printf("Name length not accepted!\n");
               }
            }

            ////////////////////////////////////
            // Check departures for that name.//
            while (pTmpDeparture != NULL) {
               iReturnCode = FindReservationOnName(&(pTmpDeparture->sReservations), szInputBuffer, &pTmpReservation);

               // Reservation on name exist.
               if (iReturnCode != ERROR) {
                  iTmpNumber++;
               }
               pTmpDeparture = pTmpDeparture->pNext;
            }

            printf("%s has total of [%d] reservations\n", szInputBuffer, iTmpNumber);

            break;
         case '9':
            printf("You quit the program\n");
            break;
         default:
            printf("Invalid input\n");
            break;
      }
   } while (cUserChoice != '9');

// Cleanup
   iReturnCode = FreeFlightDepartures(&sFlightDepartures);

   if (iReturnCode != OK) {
      pgerror("Failed to free database.");
   } else {
      pgdebug("Database was successfully free'd");
   }
   return 0;
}


void PrintMenu() {
   printf("\n+==================================================+\n");
   printf("| Type the command you want [1-6]                  |\n");
   printf("| 1. Add a flight departure.                       |\n");
   printf("| 2. Add a reservation.                            |\n");
   printf("| 3. Get info on certain departure by index.       |\n");
   printf("| 4. Find departure on certain TFD.                |\n");
   printf("| 5. Delete departure.                             |\n");
   printf("| 6. Delete reservation.                           |\n");
   printf("| 7. Change seat number.                           |\n");
   printf("| 8. Get amount of departure of certain passenger. |\n");
   printf("| 9. Quit                                          |\n");
   printf("+==================================================+\n");
}


/////////////////////////////////////////////////////////
// * Gets input from stdin and store it in the buffer. //
/////////////////////////////////////////////////////////
void GetInput(char *buffer) {
   fgets(buffer, INPUT_BUFFER_SIZE, stdin);
   // fgets stops when N-1
   // But to be sure.
   buffer[INPUT_BUFFER_SIZE - 1] = '\0';
   // Remove the newline from the input.
   buffer[strlen(buffer) - 1] = '\0';
}


////////////////////////////////////////////////////////////
// * Checks if input is valid and return the valid number //
// * loops until input is valid. (maybe not best pratice) //
////////////////////////////////////////////////////////////
int GetIntegerInput(char *szInputBuffer) {

   int iTmpNumber = 0;
   bool bInvalidInput = true;


   while (bInvalidInput) {
      ClearInputBuffer(szInputBuffer);
      GetInput(szInputBuffer);

      iTmpNumber = atoi(szInputBuffer);
      // Invalid input.
      if (IntegerInputCheck(iTmpNumber) != OK) {
         printf("Invalid input!\n");

         // Accepted input.
      } else {
         bInvalidInput = false;
      }
   }
   return iTmpNumber;
}

///////////////////////////////////////////////////////////////////////////////////
// * If number is equal  max size of integer or less than zero.                  //
//  -> Unreasonable amount or not an int and is defined as suspicious behavior.  //
//  -> This will log an error and return ERROR.                                  //
// * Return OK if not.                                                           //
///////////////////////////////////////////////////////////////////////////////////
int IntegerInputCheck(int iNumber) {
   int iReturnCode = ERROR;

   if (iNumber == INT_MAX || iNumber <= 0) {
      pgerror("Suspicious behavior detected");
   } else {
      iReturnCode = OK;
   }
   return iReturnCode;
}


///////////////////////////////////////
// * Sets all values in buffer to 0. //
// Literally memset.                 //
///////////////////////////////////////
void ClearInputBuffer(char *buffer) {
   for (int i = 0; i < strlen(buffer); i++) {
      buffer[i] = 0;
   }
}


///////////////////////////////////////////////
// * Flush reminding values in stdin buffer. //
///////////////////////////////////////////////
void FlushStdinBuffer() {
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}


int AddFlightReservation(RESERVATIONS *pReservations, char *szInputBuffer, int iNumberOfSeatsAvailable) {

   pgdebug("User entering reservation info");
   int iReturnCode = ERROR;
   int iTmpNumber = 0;
   unsigned long ulInputLength = 0;
   bool bInvalidInput = true;

   RESERVATION sTmpReservation = {0};

   printf("Enter the reservation details\n");

   // Get input for name.
   pgdebug("User entering name");

   // Loop until user enter valid length.
   while (bInvalidInput) {
      ClearInputBuffer(szInputBuffer);
      printf("Enter name (MAX = [%d]\n", NAME_MAX_LENGTH);
      GetInput(szInputBuffer);
      ulInputLength = strlen(szInputBuffer);

      if (ulInputLength <= NAME_MAX_LENGTH) {
         bInvalidInput = false;
      } else {
         printf("Name length not accepted!\n");
      }
   }

   // Allocate
   sTmpReservation.szName = (char *) malloc(NAME_MAX_LENGTH + 1);

   // Simple check
   if (sTmpReservation.szName == NULL) {
      pgerror("Something went wrong.");
      return iReturnCode;
   }
   // Clean start and Copy input to temp struct.
   memset(sTmpReservation.szName, 0, NAME_MAX_LENGTH + 1); // clean sheets.
   strncpy(sTmpReservation.szName, szInputBuffer, NAME_MAX_LENGTH);

   // Ensure zero terminator.
   sTmpReservation.szName[NAME_MAX_LENGTH] = '\0';
   pgdebug("Successfully entered name");

   // Get input for amount of seats, will keep asking until input is valid.
   pgdebug("User Entering seat number");
   bInvalidInput = true;
   printf("Enter seat number \n");
   while (bInvalidInput) {
      ClearInputBuffer(szInputBuffer);
      GetInput(szInputBuffer);

      iTmpNumber = atoi(szInputBuffer);
      // Invalid input.
      if (IntegerInputCheck(iTmpNumber) != OK) {
         printf("Invalid input!\n");

         // Seat number over the amount of seats on flight.
      } else if (iTmpNumber > iNumberOfSeatsAvailable) {
         printf("Invalid seat number MAX[%d] !\n", iNumberOfSeatsAvailable);

         // Accepted input.
      } else {
         bInvalidInput = false;
      }
   }
   sTmpReservation.iSeatNumber = iTmpNumber;
   pgdebug("Successfully entered seats");

   // Get input for age , will keep asking until input is valid.
   pgdebug("User Entering age");
   bInvalidInput = true;
   while (bInvalidInput) {
      printf("Enter your age \n");
      ClearInputBuffer(szInputBuffer);
      GetInput(szInputBuffer);

      iTmpNumber = atoi(szInputBuffer);
      // Invalid input.
      if (IntegerInputCheck(iTmpNumber) != OK) {
         printf("Invalid input!\n");

         // Age is unreasonable.
      } else if (iTmpNumber > 90 || iTmpNumber < 10) {
         printf("We cant accept reservations for people over 90 or under 10!\n");
         printf("Because of insurance and legal issues\n");

         // Accepted input.
      } else {
         bInvalidInput = false;
      }
   }
   sTmpReservation.iAge = iTmpNumber;
   pgdebug("Successfully entered age");

   // Check if reservation name or seat already exists.
   iReturnCode = CheckIfReservationExist(pReservations, sTmpReservation.szName, sTmpReservation.iSeatNumber);

   // Name exists.
   if (iReturnCode == NAME_EXISTS) {
      printf("Name already exist in the reservations\n");
      return ERROR;

      // Seat is taken.
   } else if (iReturnCode == SEAT_TAKEN) {
      printf("Seat is taken\n");
      return ERROR;

      // Seat and name is accepted.
   } else if (iReturnCode == OK) {

      // Adding the Reservation to the list.
      iReturnCode = AddReservation(pReservations,
                                   sTmpReservation.iSeatNumber,
                                   sTmpReservation.szName,
                                   sTmpReservation.iAge);
      if (iReturnCode != OK) {
         pgerror("Error adding reservation, Program terminated.");
         return iReturnCode;
      }

      // Cleanup
      free(sTmpReservation.szName);
      sTmpReservation.szName = NULL;
      memset(&sTmpReservation, 0, sizeof(RESERVATION));

      printf("Successfully created reservation!\n");
   }


   return iReturnCode;
}

//////////////////////////////////////////////////////////////
// * Adds a departure to the list and handles the proper way//
// * Returns code that indicate success or error.           //
//////////////////////////////////////////////////////////////
int AddFlightDeparture(FLIGHT_DEPARTURES *pFlightDepartures, char *szInputBuffer) {
   int iReturnCode = ERROR;
   int iTmpNumber = 0;
   unsigned long ulInputLength = 0;
   bool bInvalidInput = true;

   DEPARTURE sTmpDeparture = {0};

   // Get input for flight id.
   pgdebug("User entering flight id");
   ClearInputBuffer(szInputBuffer);
   printf("Enter Flight ID\n");
   GetInput(szInputBuffer);
   ulInputLength = strlen(szInputBuffer);
   sTmpDeparture.szFlightID = (char *) malloc(ulInputLength + 1); // +1 for terminator.

   // Simple check.
   if (sTmpDeparture.szFlightID == NULL) {
      pgerror("Something went wrong.");
      return iReturnCode;
   }

   // Clean start and Copy input to temp struct.
   memset(sTmpDeparture.szFlightID, 0, ulInputLength + 1); // clean sheets.
   strncpy(sTmpDeparture.szFlightID, szInputBuffer, ulInputLength);

   // Ensure zero terminator.
   sTmpDeparture.szFlightID[ulInputLength] = '\0';
   pgdebug("Successfully entered flight id");

   // Get input for destination.
   pgdebug("User entering destination");
   ClearInputBuffer(szInputBuffer);
   printf("Enter Destination\n");
   GetInput(szInputBuffer);
   sTmpDeparture.szDestination = (char *) malloc(strlen(szInputBuffer) + 1);

   // Simple check.
   if (sTmpDeparture.szFlightID == NULL) {
      pgerror("Something went wrong.");
      return iReturnCode;
   }

   // Clean start and Copy input to temp struct.
   memset(sTmpDeparture.szDestination, 0, strlen(szInputBuffer) + 1);
   strncpy(sTmpDeparture.szDestination, szInputBuffer, strlen(szInputBuffer));
   pgdebug("Successfully entered destination");

   // Get input for amount of seats, will keep asking until input is valid.
   pgdebug("User Entering seats");
   bInvalidInput = true;
   while (bInvalidInput) {
      printf("Enter the amount of seats\n");
      ClearInputBuffer(szInputBuffer);
      GetInput(szInputBuffer);

      iTmpNumber = atoi(szInputBuffer);
      if (IntegerInputCheck(iTmpNumber) != OK) {
         printf("Invalid input!\n");
      } else {
         bInvalidInput = false;
      }
   }
   sTmpDeparture.iSeats = iTmpNumber;
   pgdebug("Successfully entered seats");

   // Get input for TFD, will keep asking until input is valid.
   pgdebug("User entering TFD");
   printf("Enter the Time for departure\n");
   printf("Format 0001 - 2400\n");
   bInvalidInput = true;
   while (bInvalidInput) {
      iTmpNumber = 0;
      ClearInputBuffer(szInputBuffer);
      GetInput(szInputBuffer);

      iTmpNumber = atoi(szInputBuffer);

      if (IntegerInputCheck(iTmpNumber) != OK) {
         printf("Invalid input!\n");
      } else if (iTmpNumber > 2400) {
         printf("Warning, Kindly enter time in format 0001 - 2400\n");
      } else {
         bInvalidInput = false;
      }
   }
   sTmpDeparture.iTFD = iTmpNumber;
   pgdebug("Successfully entered TFD");

   // Adding the departure to the list.
   iReturnCode = AddDeparture(pFlightDepartures,
                              sTmpDeparture.szFlightID,
                              sTmpDeparture.szDestination,
                              sTmpDeparture.iSeats,
                              sTmpDeparture.iTFD);
   if (iReturnCode != OK) {
      pgerror("Error adding departure, Program terminated.");
      return iReturnCode;
   }

   //Log progress, Cleanup the temps and free allocated space.
   pgdebug("Successfully added departure");
   free(sTmpDeparture.szFlightID);
   free(sTmpDeparture.szDestination);
   memset(&sTmpDeparture, 0, sizeof(DEPARTURE));
   iTmpNumber = 0;
   bInvalidInput = true;

   return iReturnCode;
}


///////////////////////////////////////////////////////////////////////
// * Checks the list for the entry with "index" update result pointer//
// * Returns code that indicate success or error.                    //
///////////////////////////////////////////////////////////////////////
int GetDepartureOnIndex(FLIGHT_DEPARTURES *pFlightDepartures, char *szInputBuffer, DEPARTURE **ppDepartureResult) {
   int iReturnCode = ERROR;
   int iTmpNumber = 0;
   bool bInvalidInput = true;
   DEPARTURE *pThis;

   pgdebug("Starting to find departure on index");

   // Check if list is empty.
   if (pFlightDepartures->iSize != 0) {

      // Runs until user enter valid input.
      while (bInvalidInput) {
         printf("Enter the index of the departure [1] - [%d](currentMax)\n", pFlightDepartures->iSize);
         iTmpNumber = 0;
         ClearInputBuffer(szInputBuffer);
         GetInput(szInputBuffer);

         iTmpNumber = atoi(szInputBuffer);

         if (IntegerInputCheck(iTmpNumber) != OK) {
            printf("Invalid input!\n");

            // Check if index is out of bounds, i check this in FindDepartureOnIndex
            // , but dont want to run it unnecessary many times.
         } else if (iTmpNumber < 1 || iTmpNumber > (pFlightDepartures->iSize)) {
            printf("No departure at that index.\n");

            // Exit while loop.
         } else {
            bInvalidInput = false;
         }
      }
   } else {
      printf("No departures available!\n");
      return ERROR;
   }

   // Find the departure on index chosen by user and update result.
   iReturnCode = FindDepartureOnIndex(pFlightDepartures, iTmpNumber, ppDepartureResult);
   if (iReturnCode != OK) {
      pgerror("Something went wrong.");
      return ERROR;
   }

   pgdebug("Index [%d] successfully found", iTmpNumber);

   // Cleanup
   iTmpNumber = 0;
   bInvalidInput = true;

   return iReturnCode;
}


int PrintDeparture(DEPARTURE *pDeparture) {
   int iReturnCode = ERROR;

   // Print all info of a departure.
   iReturnCode = PrintDepartureInfo(pDeparture);
   if (iReturnCode != OK) {
      pgerror("Something went wrong.");
      return ERROR;
   }

   return iReturnCode;
}