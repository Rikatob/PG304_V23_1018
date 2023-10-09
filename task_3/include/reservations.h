

#ifndef _TASK_3_RESERVATIONS_H_
#define _TASK_3_RESERVATIONS_H_

#define OK      0
#define ERROR   1
#define NAME_EXISTS 2
#define SEAT_TAKEN 3

#define NAME_MAX_LENGTH 20

typedef struct _RESERVATION {
   int iSeatNumber;
   char *szName;
   int iAge;
   struct _RESERVATION *pNext;
} RESERVATION;

typedef struct _RESERVATIONS {
   RESERVATION *pHead;
   int iSize;
} RESERVATIONS;

////////////////////////////////////////////FUNCTION PROTOTYPES////////////////////////////////////////////

int PrintReservationsInfo(RESERVATIONS *pReservations);

int FreeReservations(RESERVATIONS *pnReservations);

int FreeReservation(RESERVATION *pnReservation);

int AddReservation(RESERVATIONS *pnReservations, int iSeatNumber, char *szName, int iAge);

int RemoveReservation(RESERVATIONS *pnReservations, char *szName);

int ChangeSeatNumber(RESERVATIONS *pnReservations, char *szName, int iNewSeatNumber);

int FindReservationOnName(RESERVATIONS *pnReservations, char *szName, RESERVATION **ppnReservationResult);

RESERVATION *CreateReservation(int iSeatNumber, char *szName, int iAge);

int CheckIfReservationExist(RESERVATIONS *pnReservations, char *szName, int iSeatNumber);

#endif //_TASK_3_RESERVATIONS_H_
