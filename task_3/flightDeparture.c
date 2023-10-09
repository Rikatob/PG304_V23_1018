

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flightDeparture.h"
#include "pgdbglog.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * Args                                                                                                   //
//  -> List to check, FlightID to identify the departure and a p to p to where the result should be updated.//
// * Checks the list for an entry with the Flight ID and update result.                                     //
// * returns OK if Departure is found and ERROR if not.                                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FindDepartureOnFlightId(FLIGHT_DEPARTURES *pnFlightDepartures, char *szFlightId, DEPARTURE **ppnDepartureResult) {
   int iReturnCode = ERROR;
   DEPARTURE *pnDeparture;

   // check If list is empty.
   if (pnFlightDepartures->pHead == NULL) {
      pgerror("Tried to find entry in empty database.");
   } else {
      // Check if ID is tail -> set pnDeparture to pTail.
      if (strncasecmp(pnFlightDepartures->pTail->szFlightID, szFlightId, MAX_ID_LENGTH - 1) == 0) {
         pnDeparture = pnFlightDepartures->pTail;
         // Simple check.
         if (pnDeparture != NULL) {
            iReturnCode = OK;
         }

         // Check if ID is head -> set pnDeparture to pHead.
      } else if (strncasecmp(pnFlightDepartures->pHead->szFlightID, szFlightId, MAX_ID_LENGTH - 1) == 0) {
         pnDeparture = pnFlightDepartures->pHead;
         // Simple check.
         if (pnDeparture != NULL) {
            iReturnCode = OK;
         }

         // Loops through the list from head and sets pnDeparture to current node if ID is same as argument ID.
      } else {
         pnDeparture = pnFlightDepartures->pHead;
         while (pnDeparture != NULL) {
            if (strncasecmp(pnDeparture->szFlightID, szFlightId, MAX_ID_LENGTH - 1) == 0) {
               // Simple check.
               if (pnDeparture != NULL) {
                  iReturnCode = OK;
               }
               break;
            }
            pnDeparture = pnDeparture->pNext;
         }
      }
      // dereference the ppnDepartureResult and set it to pnDeparture if a result was found.
      if (pnDeparture != NULL) {
         *ppnDepartureResult = pnDeparture;
      }
   }
   // If node was found return OK if not return ERROR.
   return iReturnCode;
}

int FindDepartureOnTFD(FLIGHT_DEPARTURES *pnFlightDepartures, int iTFD, int *iIndex) {
   int iReturnCode = ERROR;
   int iTempIndex = 1; // Index 0 is faulty , list start at index as 1.
   DEPARTURE *pnDeparture = pnFlightDepartures->pHead;

   // Check If list is empty.
   if (pnFlightDepartures->pHead == NULL) {
      pgerror("Tried to find entry in empty database.");

   } else if (pnDeparture != NULL) {

      while (pnDeparture != NULL) {
         if (pnDeparture->iTFD == iTFD) {
            *iIndex = iTempIndex;
            iReturnCode = OK;
            break;
         }
         iTempIndex++;
         pnDeparture = pnDeparture->pNext;
      }
   }
   return iReturnCode;
}

////////////////////////////////////////////////////////////////////////////////
// * Equivalent to FindReservationOnName just using index to find departure.//
////////////////////////////////////////////////////////////////////////////////
int FindDepartureOnIndex(FLIGHT_DEPARTURES *pnFlightDepartures, int iIndex, DEPARTURE **ppnDepartureResult) {
   int iReturnCode = ERROR;
   int iTempIndex = 1; // Index 0 is faulty , list start at index as 1.
   DEPARTURE *pnDeparture;

   // Check If list is empty.
   if (pnFlightDepartures->pHead == NULL) {
      pgerror("Tried to find entry in empty database.");

      // Check if index is out of bounds.
   } else if (iIndex <= 0 || iIndex > pnFlightDepartures->iSize) {
      pgerror("Tried to find index out of bounds.");

   } else {
      // Check if index is tail -> set pnDeparture to pTail.
      if (iIndex == (pnFlightDepartures->iSize)) {
         pnDeparture = pnFlightDepartures->pTail;
         // Simple check.
         if (pnDeparture != NULL) {
            iReturnCode = OK;
         }

         // Check if index is head -> set pnDeparture to pHead.
      } else if (iIndex == 1) {
         pnDeparture = pnFlightDepartures->pHead;
         // Simple check.
         if (pnDeparture != NULL) {
            iReturnCode = OK;
         }

         // Loops through the list from head and sets pnDeparture to current node's index is iIndex.
      } else {
         pnDeparture = pnFlightDepartures->pHead;
         while (pnDeparture != NULL) {

            if (iTempIndex == iIndex) {
               // Simple check.
               if (pnDeparture != NULL) {
                  iReturnCode = OK;
               }
               break;
            }
            pnDeparture = pnDeparture->pNext;
            iTempIndex++;
         }
      }
      // dereference the ppnDepartureResult and set it to pnDeparture if a result was found.
      if (pnDeparture != NULL) {
         *ppnDepartureResult = pnDeparture;
      }
   }

// If node was found return OK if not return ERROR.
   return iReturnCode;
}

////////////////////////////////////////////////////////////////////////////
// * Args                                                                 //
// -> list to add the departure to and the values needed to create it.    //
// * Add the departure to the list in the proper way.                     //
// * return OK if success and ERROR if not.                               //
// * Increment list size for every successfully added departure.          //
////////////////////////////////////////////////////////////////////////////
int AddDeparture(FLIGHT_DEPARTURES *pnFlightDepartures, char *szId, char *szDestination, int iSeats, int iTFD) {
   int iReturnCode = ERROR;
   pgdebug("Started adding departure.");

   DEPARTURE *pnDeparture = CreateDeparture(szId, szDestination, iSeats, iTFD);
   if (pnDeparture != NULL) {

      // List is empty.
      if (pnFlightDepartures->pHead == NULL && pnFlightDepartures->pTail == NULL) {
         pnFlightDepartures->pHead = pnDeparture;
         pnFlightDepartures->pTail = pnDeparture;
         iReturnCode = OK;

         // Add to the end of the list.
      } else {
         pnFlightDepartures->pTail->pNext = pnDeparture;
         pnDeparture->pPrev = pnFlightDepartures->pTail;
         pnFlightDepartures->pTail = pnDeparture;
         iReturnCode = OK;
      }
   }

   // Increment size if departure is added.
   if (iReturnCode == OK) {
      pnFlightDepartures->iSize++;

   } else {
      pgerror("Departure failed to be added to list.");
   }

   return iReturnCode;
}


//////////////////////////////////////////////////////////////
// * Args                                                   //
//  -> values to create a departure.                        //
// * allocate space in a proper way and sets the values.    //
// * returns a pointer to the created departure.            //
//////////////////////////////////////////////////////////////
DEPARTURE *CreateDeparture(char *szId, char *szDestination, int iSeats, int iTFD) {

   pgdebug("Started creating departure");

   // Find lengths.
   unsigned long ulIdLength = strnlen(szId, sizeof(unsigned long));
   unsigned long ulDestinationLength = strnlen(szDestination, sizeof(unsigned long));

   // Allocate
   DEPARTURE *pnDeparture = (DEPARTURE *) malloc(sizeof(DEPARTURE));

   if (pnDeparture == NULL) {
      pgerror("Departure failed to create.");

   } else {
      // Clean sheets.
      memset(pnDeparture, 0, sizeof(DEPARTURE));

      // Allocate space for values in departure.
      pnDeparture->szFlightID = (char *) malloc(ulIdLength + 1);
      pnDeparture->szDestination = (char *) malloc(ulDestinationLength + 1);

      // Set values in departure.
      strncpy(pnDeparture->szFlightID, szId, ulIdLength);
      strncpy(pnDeparture->szDestination, szDestination, ulDestinationLength);
      pnDeparture->iSeats = iSeats;
      pnDeparture->iTFD = iTFD;

      // Ensure zero termination.
      pnDeparture->szFlightID[ulIdLength] = '\0';
      pnDeparture->szDestination[ulDestinationLength] = '\0';
      pgdebug("Departure with flight ID[%s] successfully created", pnDeparture->szFlightID)
   }

   return pnDeparture;
}

int PrintDepartureInfo(DEPARTURE *pnDeparture) {
   pgdebug("Started printing departure");
   int iReturnCode = ERROR;

   // Setting up temp variables to make it easier to print.
   // Departure.
   char *szID = pnDeparture->szFlightID;
   char *szDestination = pnDeparture->szDestination;
   int iSeats = pnDeparture->iSeats;
   int iTFD = pnDeparture->iTFD;

   // Print format.
   printf("+====================================+\n");
   printf("| * Flight id = [%s]\n", szID);
   printf("| * Destination = [%s]\n", szDestination);
   printf("| * Number of seats = [%d]\n", iSeats);
   printf("| * Time for departure = [%04d]\n", iTFD);
   printf("| * Reservations\n");

   // Print reservations, if any, and message to indicate no reservations if empty.
   iReturnCode = PrintReservationsInfo(&(pnDeparture->sReservations));
   if (iReturnCode != OK) {
      pgerror("Something went wrong printing reservations.");
      return iReturnCode;
   }

   printf("+====================================+\n");
   pgdebug("Successfully printed departure.");
   return iReturnCode;
}


//////////////////////////////////////////////////////////////////////////
// * Search for the departure with the id from argument.                //
// * Remove and free the departure if it exists.                        //
// * Return OK if successfully deleted or error if departure not found. //
//////////////////////////////////////////////////////////////////////////
int RemoveDeparture(FLIGHT_DEPARTURES *pnFlightDepartures, char *szFlightId) {
   int iReturnCode = ERROR;
   int iEntryCountBefore = pnFlightDepartures->iSize;
   int iEntryCountAfter;
   // temp.
   DEPARTURE *pnDeparture;

   iReturnCode = FindDepartureOnFlightId(pnFlightDepartures, szFlightId, &pnDeparture);

   if (iReturnCode == OK) {
      iReturnCode = ERROR;

      // Only one departure in list.
      if (pnFlightDepartures->pHead == pnFlightDepartures->pTail) {
         pnFlightDepartures->pHead = NULL;
         pnFlightDepartures->pTail = NULL;
         FreeDeparture(pnDeparture);
         pnFlightDepartures->iSize--;
         if (pnFlightDepartures->iSize == 0) {
            pgdebug("Last entry in departures is successfully deleted.")
            iReturnCode = OK;
         }
      }
         // Departure to delete is head.
      else if (pnDeparture == pnFlightDepartures->pHead) {
         pnDeparture->pNext->pPrev = NULL;
         pnFlightDepartures->pHead = pnDeparture->pNext;
         pnFlightDepartures->iSize--;

         // Verify remove opperation with size.
         iEntryCountAfter = pnFlightDepartures->iSize;
         if (iEntryCountBefore - 1 == iEntryCountAfter) {
            iReturnCode = FreeDeparture(pnDeparture);
         }
         // Entry to delete is tail.
      } else if (pnDeparture == pnFlightDepartures->pTail) {
         pnDeparture->pPrev->pNext = NULL;
         pnFlightDepartures->pTail = pnDeparture->pPrev;
         pnFlightDepartures->iSize--;
         iEntryCountAfter = pnFlightDepartures->iSize;

         // Verify remove opperation with size.
         if (iEntryCountBefore - 1 == iEntryCountAfter) {
            iReturnCode = FreeDeparture(pnDeparture);
         }
         // Entry to delete is "in the middle".
      } else {
         pnDeparture->pNext->pPrev = pnDeparture->pPrev;
         pnDeparture->pPrev->pNext = pnDeparture->pNext;
         pnFlightDepartures->iSize--;

         // Verify remove opperation with size.
         iEntryCountAfter = pnFlightDepartures->iSize;
         if (iEntryCountBefore - 1 == iEntryCountAfter) {
            iReturnCode = FreeDeparture(pnDeparture);
         }
      }
   }
   pnDeparture = NULL;
   return iReturnCode;
}

int FreeDeparture(DEPARTURE *pnDeparture) {
   int iReturnCode = ERROR;

   if (pnDeparture != NULL) {
      free(pnDeparture->szFlightID);
      free(pnDeparture->szDestination);
      pnDeparture->szFlightID = NULL;
      pnDeparture->szDestination = NULL;
      pnDeparture->iSeats = -1;
      pnDeparture->iTFD = -1;
      pnDeparture->pNext = NULL;
      pnDeparture->pPrev = NULL;

      iReturnCode = FreeReservations(&(pnDeparture->sReservations));
      if (iReturnCode != OK) {
         pgerror("Something went wrong free'ing reservations.");
         return ERROR;
      } else {
         iReturnCode = OK;
      }

      memset(pnDeparture, 0, sizeof(DEPARTURE));
      free(pnDeparture);
      pnDeparture = NULL;
   }

   return iReturnCode;
}

int FreeFlightDepartures(FLIGHT_DEPARTURES *pnFlightDepartures) {
   int iReturnCode = ERROR;
   DEPARTURE *pnDeparture = pnFlightDepartures->pHead;

   //List is empty.
   if (pnFlightDepartures->iSize == 0) {
      pnDeparture = NULL;
      iReturnCode = OK;

      // Only one entry.
   } else if (pnFlightDepartures->iSize == 1) {
      iReturnCode = FreeDeparture(pnDeparture);

      // Two or more entries.
   } else {
      if (pnDeparture != NULL) {

         // Lopp thorugh the departures and free them until one element left.
         while (pnDeparture->pNext != NULL) {
            pnDeparture = pnDeparture->pNext;
            iReturnCode = FreeDeparture(pnDeparture->pPrev);

            if (iReturnCode != OK) {
               return iReturnCode;
            }
         }
         // Free last
         iReturnCode = FreeDeparture(pnDeparture);

         // Cleanup
         pnFlightDepartures->pHead = NULL;
         pnFlightDepartures->pTail = NULL;
         pnDeparture = NULL;
         memset(pnFlightDepartures, 0, sizeof(FLIGHT_DEPARTURES));
      }
   }
   return iReturnCode;
}
