

#ifndef _TASK_3_FLIGHTDEPARTURE_H_
#define _TASK_3_FLIGHTDEPARTURE_H_

#include "reservations.h"

#define OK      0
#define ERROR   1
#define MAX_ID_LENGTH 10
#define MAX_DESTINATION_LENGTH 30


typedef struct _DEPARTURE {
   char *szFlightID;
   char *szDestination;
   int iSeats;
   int iTFD; // Time for departure.
   struct _RESERVATIONS sReservations;
   struct _DEPARTURE *pNext;
   struct _DEPARTURE *pPrev;
} DEPARTURE;

typedef struct _FLIGHT_DEPARTURES {
   DEPARTURE *pHead;
   DEPARTURE *pTail;
   int iSize;

} FLIGHT_DEPARTURES;

////////////////////////////////////////////FUNCTION PROTOTYPES////////////////////////////////////////////
DEPARTURE *CreateDeparture(char *szId, char *szDestination, int iSeats, int iTFD);

int FindDepartureOnFlightId(FLIGHT_DEPARTURES *pnFlightDepartures, char *szFlightId, DEPARTURE **ppnDepartureResult);

int FindDepartureOnIndex(FLIGHT_DEPARTURES *pnFlightDepartures, int iIndex, DEPARTURE **ppnDepartureResult);

int AddDeparture(FLIGHT_DEPARTURES *pnFlightDepartures, char *szId, char *szDestination, int iSeats, int iTFD);

int PrintDepartureInfo(DEPARTURE *pnDeparture);

int RemoveDeparture(FLIGHT_DEPARTURES *pnFlightDepartures, char *szFlightId);

int FreeDeparture(DEPARTURE *pnDeparture);

int FindDepartureOnTFD(FLIGHT_DEPARTURES *pnFlightDepartures, int iTFD, int *iIndex);

int FreeFlightDepartures(FLIGHT_DEPARTURES *pnFlightDepartures);


#endif //_TASK_3_FLIGHTDEPARTURE_H_
