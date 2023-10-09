
#ifndef _TASK_3_SOURCE_H_
#define _TASK_3_SOURCE_H_

#include "flightDeparture.h"

#define OK 0
#define ERROR 1
#define INPUT_BUFFER_SIZE 128

void PrintMenu();

void ClearInputBuffer(char *buffer);

void GetInput(char *buffer);

void FlushStdinBuffer();

int IntegerInputCheck(int iNumber);

int GetIntegerInput(char *szInputBuffer);

int AddFlightDeparture(FLIGHT_DEPARTURES *pFlightDepartures, char *szInputBuffer);

int GetDepartureOnIndex(FLIGHT_DEPARTURES *pFlightDepartures, char *szInputBuffer, DEPARTURE **ppDepartureResult);

int AddFlightReservation(RESERVATIONS *pReservations, char *szInputBuffer, int iNumberOfSeatsAvailable);

int PrintDeparture(DEPARTURE *pDeparture);

#endif //_TASK_3_SOURCE_H_
