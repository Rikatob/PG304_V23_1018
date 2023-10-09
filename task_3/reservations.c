
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reservations.h"
#include "pgdbglog.h"

int PrintReservationsInfo(RESERVATIONS *pReservations) {

   int iReturnCode = ERROR;
   int iIndex = 1;
   RESERVATION *pnReservation = pReservations->pHead;

   if (pnReservation != NULL) {
      while (pnReservation != NULL) {
         printf("| [Reservation number %d]\n", iIndex);
         printf("|  -> Seatnumber = [%d]\n", pnReservation->iSeatNumber);
         printf("|  -> Name = [%s]\n", pnReservation->szName);
         printf("|  -> Age = [%d]\n", pnReservation->iAge);
         pnReservation = pnReservation->pNext;
         iIndex++;
      }
      // If pnReservation is size of reservations, then all reservations has been printed.
      if (iIndex == pReservations->iSize) {
         iReturnCode = OK;
      }
      // Empty list , print accordingly.
   } else if (pReservations->pHead == NULL) {
      printf("|  +=====================+\n");
      printf("|  |***No reservations***|\n");
      printf("|  +=====================+\n");
      iReturnCode = OK;
   }
   return iReturnCode;
}


/////////////////////////////////////////////////////////////////
// * Check if name or seat already exists in reservations.     //
// * Return                                                    //
//  -> NAME_EXISTS                                             //
//  -> SEAT_TAKEN                                              //
//  -> OK if it dosent exist.                                  //
//  -> ERROR if something went wrong.                          //
/////////////////////////////////////////////////////////////////
int CheckIfReservationExist(RESERVATIONS *pnReservations, char *szName, int iSeatNumber) {
   int iReturnCode = ERROR;

   // If list is empty -> should not check if exist.
   if (pnReservations->iSize != 0) {
      RESERVATION *pnReservation = pnReservations->pHead;

      if (pnReservation != NULL) {

         while (pnReservation != NULL) {
            // Name exists.
            if (strncasecmp(pnReservation->szName, szName, NAME_MAX_LENGTH) == 0) {
               iReturnCode = NAME_EXISTS;
               break;
               // Set is taken.
            } else if (pnReservation->iSeatNumber == iSeatNumber) {
               iReturnCode = SEAT_TAKEN;
               break;
               // No crashes on existing reservation.
            } else {
               iReturnCode = OK;
               pnReservation = pnReservation->pNext;
            }
         }
      }
   } else {
      iReturnCode = OK;
   }
   return iReturnCode;
}


/////////////////////////////////////////////////
// * Change seat number on certain reservation.//
// * Return                                    //
//  -> OK, everything accepted.                //
//  -> SEAT_TAKEN, seat not available.         //
//  -> ERROR, something went wrong.            //
/////////////////////////////////////////////////
int ChangeSeatNumber(RESERVATIONS *pnReservations, char *szName, int iNewSeatNumber) {
   int iReturnCode = ERROR;
   int iTempAge = 0;


   RESERVATION *pnReservation;

   // Check if seat is taken.
   iReturnCode = CheckIfReservationExist(pnReservations, szName, iNewSeatNumber);

   // Seat not taken.
   if (iReturnCode != SEAT_TAKEN) {

      // Find reservation.
      iReturnCode = FindReservationOnName(pnReservations, szName, &pnReservation);
      if (iReturnCode == ERROR) {
         return iReturnCode;
      }

      // Store age before removing.
      iTempAge = pnReservation->iAge;

      // Remove old.
      iReturnCode = RemoveReservation(pnReservations, szName);
      if (iReturnCode == ERROR) {
         return iReturnCode;
      }

      // Add new with updated seatnumber.
      iReturnCode = AddReservation(pnReservations, iNewSeatNumber, szName, iTempAge);
      if (iReturnCode == ERROR) {
         return iReturnCode;
      }
   }

   return iReturnCode;
}


int AddReservation(RESERVATIONS *pnReservations, int iSeatNumber, char *szName, int iAge) {
   int iReturnCode = ERROR;

   RESERVATION *pnCreated = CreateReservation(iSeatNumber, szName, iAge);

   if (pnCreated != NULL) {

      // List is empty
      if (pnReservations->iSize == 0) {
         pnReservations->pHead = pnCreated;
         pnReservations->iSize++;
         iReturnCode = OK;

         // Insert in front.
      } else if (iSeatNumber < pnReservations->pHead->iSeatNumber) {
         pnCreated->pNext = pnReservations->pHead;
         pnReservations->pHead = pnCreated;
         pnReservations->iSize++;
         iReturnCode = OK;

         // Only one entry
      } else if (pnReservations->iSize == 1) {
         // insert after head.
         if (pnCreated->iSeatNumber > pnReservations->pHead->iSeatNumber) {
            pnReservations->pHead->pNext = pnCreated;
            pnReservations->iSize++;
            iReturnCode = OK;
            // Inser after head.
         } else if (pnCreated->iSeatNumber < pnReservations->pHead->iSeatNumber) {
            pnCreated->pNext = pnReservations->pHead;
            pnReservations->pHead = pnCreated;
            pnReservations->iSize++;
            iReturnCode = OK;
         }

      } else {
         // Temps
         RESERVATION *pnReservation = pnReservations->pHead;
         RESERVATION *pnPrevReservation;

         if (pnReservation != NULL) {
            while (pnReservation->pNext != NULL) {
               pnPrevReservation = pnReservation;
               pnReservation = pnReservation->pNext;

               // Insert in the middle.
               if (pnPrevReservation->iSeatNumber < iSeatNumber && pnReservation->iSeatNumber > iSeatNumber) {
                  pnCreated->pNext = pnPrevReservation->pNext;
                  pnPrevReservation->pNext = pnCreated;
                  pnReservations->iSize++;
                  iReturnCode = OK;
                  break;

                  // Insert in end.
               } else if (pnReservation->pNext == NULL) {
                  pnReservation->pNext = pnCreated;
                  pnReservations->iSize++;
                  iReturnCode = OK;
                  break;
               }
            }
         }
      }
   }

   return iReturnCode;
}

RESERVATION *CreateReservation(int iSeatNumber, char *szName, int iAge) {
   pgdebug("Started creating reservation");
   RESERVATION *pnReservation = (RESERVATION *) malloc(sizeof(RESERVATION));
   unsigned long ulNameLength = strlen(szName);


   // If length is to long return NULL (is also checked that not more than name max length when user enter).
   if (ulNameLength <= NAME_MAX_LENGTH) {


      if (pnReservation == NULL) {
         pgerror("Reservation failed to create.");

      } else {
         memset(pnReservation, 0, sizeof(RESERVATION));

         // Allocate space for name in reservation.
         pnReservation->szName = (char *) malloc(NAME_MAX_LENGTH + 1);
         // Clean sheets and ensure terminator.
         memset(pnReservation->szName, 0, NAME_MAX_LENGTH + 1);

         // Set values from param.
         pnReservation->iSeatNumber = iSeatNumber;
         strncpy(pnReservation->szName, szName, ulNameLength);
         pnReservation->iAge = iAge;

         //Double ensure :).
         pnReservation->szName[ulNameLength] = '\0';

         pgdebug("Reservation on name [%s] successfully created.", pnReservation->szName);
      }
   } else {
      return NULL;
   }
   return pnReservation;
}


////////////////////////////////////////////////////////////////////////
// * Find reservation on name and update result pointer from argument.//
// * return OK if found or error if not found.                        //
////////////////////////////////////////////////////////////////////////
int FindReservationOnName(RESERVATIONS *pnReservations, char *szName, RESERVATION **ppnReservationResult) {
   int iReturnCode = ERROR;
   // Temp
   RESERVATION *pnReservation;

   // check If list is empty.
   if (pnReservations->pHead == NULL) {
      pgerror("Tried to find entry in empty List (reservations).");
   } else {

      // Check if reservation is head -> set pnReservation to pHead.
      if (strncasecmp(pnReservations->pHead->szName, szName, NAME_MAX_LENGTH) == 0) {
         pnReservation = pnReservations->pHead;
         // Simple check.
         if (pnReservation != NULL) {
            iReturnCode = OK;
         }

         // Loops through the list from head and sets pnReservation to current node if name is same as argument.
      } else {
         pnReservation = pnReservations->pHead;
         while (pnReservation != NULL) {
            if (strncasecmp(pnReservation->szName, szName, NAME_MAX_LENGTH) == 0) {
               // Simple check.
               if (pnReservation != NULL) {
                  iReturnCode = OK;
               }
               break;
            }
            pnReservation = pnReservation->pNext;
         }
      }
      // Dereference the ppnReservationResult and set it to pnReservation if a result was found.
      if (pnReservation != NULL) {
         *ppnReservationResult = pnReservation;
      }
   }
   // If node was found return OK if not return ERROR.
   return iReturnCode;
}

/////////////////////////////////////////////////////////////////
// * Find and remove a reservation on name.                    //
// * Return OK if reservation is is found and removed          //
//   and ERROR if reservation was not found.                   //
/////////////////////////////////////////////////////////////////
int RemoveReservation(RESERVATIONS *pnReservations, char *szName) {
   int iReturnCode = ERROR;
   int iEntryCountBefore = pnReservations->iSize;
   int iEntryCountAfter;
   unsigned long ulNameLength = strlen(szName);
   // Temps.
   RESERVATION *pnPrevReservation;
   RESERVATION *pnReservation = pnReservations->pHead;

   if (ulNameLength > NAME_MAX_LENGTH) {
      iReturnCode = ERROR;
   } else {

      if (pnReservation != NULL) {

         // Only one reservation in list.
         if (pnReservations->iSize == 1) {
            iReturnCode = FreeReservation(pnReservation);
            if (iReturnCode == ERROR) {
               return ERROR;
            }
            pnReservations->iSize--;
            if (pnReservations->iSize == 0) {
               pgdebug("Last entry in reservations is successfully deleted.")
               iReturnCode = OK;
            }

            // Reservation to delete is head.
         } else if (strncasecmp(pnReservation->szName, szName, ulNameLength) == 0) {
            pnReservations->pHead = pnReservation->pNext;
            pnReservations->iSize--;


            // Verify remove operation with size.
            iEntryCountAfter = pnReservations->iSize;
            if (iEntryCountBefore - 1 == iEntryCountAfter) {
               iReturnCode = FreeReservation(pnReservation);
            }

            // Reservation to delete is middle or last
         } else {

            while (pnReservation->pNext != NULL) {
               pnPrevReservation = pnReservation;
               pnReservation = pnReservation->pNext;
               if (strncasecmp(pnReservation->szName, szName, ulNameLength) == 0) {
                  iReturnCode = OK;
                  break;
               }
            }
            // Reservation found.
            if (iReturnCode == OK) {
               iReturnCode = ERROR;

               // Delete last.
               if (pnReservation->pNext == NULL) {
                  pnPrevReservation->pNext = NULL;
                  pnReservations->iSize--;

                  // Verify remove operation with size.
                  iEntryCountAfter = pnReservations->iSize;
                  if (iEntryCountBefore - 1 == iEntryCountAfter) {
                     iReturnCode = FreeReservation(pnReservation);
                  }

                  // Delete middle.
               } else {
                  pnPrevReservation->pNext = pnReservation->pNext;
                  pnReservations->iSize--;

                  // Verify remove operation with size.
                  iEntryCountAfter = pnReservations->iSize;
                  if (iEntryCountBefore - 1 == iEntryCountAfter) {
                     iReturnCode = FreeReservation(pnReservation);
                  }
               }
            }
         }
      }
   }
   return iReturnCode;
}

int FreeReservation(RESERVATION *pnReservation) {
   int iReturnCode = ERROR;

   if (pnReservation != NULL) {
      free(pnReservation->szName);
      memset(pnReservation, 0, sizeof(RESERVATION));
      free(pnReservation);
      pnReservation = NULL;
      iReturnCode = OK;
   }

   return iReturnCode;
}

int FreeReservations(RESERVATIONS *pnReservations) {
   int iReturnCode = ERROR;
   // Temps.
   RESERVATION *pnReservation = pnReservations->pHead;
   RESERVATION *pnPrevReservation;

   //List is empty.
   if (pnReservations->iSize == 0) {
      pnReservation = NULL;
      iReturnCode = OK;

      // Only one entry.
   } else if (pnReservations->iSize == 1) {
      //iReturnCode = RemoveReservation(pnReservations, pnReservation->szName);
      iReturnCode = FreeReservation(pnReservation);

      // Two or more entries.
   } else {
      if (pnReservation != NULL) {

         // Removes all except the last reservation.
         while (pnReservation->pNext != NULL) {
            pnPrevReservation = pnReservation;
            pnReservation = pnReservation->pNext;
            //iReturnCode = RemoveReservation(pnReservations, pnPrevReservation->szName);
            iReturnCode = FreeReservation(pnPrevReservation);

            if (iReturnCode != OK) {
               return iReturnCode;
            }
         }
         // Remove last
         if (iReturnCode == OK) {
            //iReturnCode = RemoveReservation(pnReservations, pnReservation->szName);
            iReturnCode = FreeReservation(pnReservation);
         }


         // Cleanup.
         if (iReturnCode == OK) {
            pnReservation = NULL;
            pnPrevReservation = NULL;
            memset(pnReservations, 0, sizeof(RESERVATIONS));
            pnReservations->pHead = NULL;
         }
      }
   }
   return iReturnCode;
}
