#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runway.h"
#include "flight.h"
#ifndef _runway_C_
#define _runway_C_


typedef struct FlightsList{
	pflightElement flightData;
	struct FlightsList* pNextNode; 
} flighsList ;

typedef struct NewRunway {
	int RunwayNum;
	FlightType destination;  //international or domestic
	struct FlightsList* pflightLinker;
} Runway;


//**************************************************************
//* Function name: createRunway
//* Description: creates a new Runway with the needed information.
//* Parameters: Runway number, Runway type.
//* return value: a pointer to the new Runway or NULL in case the function failed.
//**************************************************************

pRunway createRunway(int runwayNum, FlightType Dest) {
	if ((!(DataIsValid(runwayNum))) || (!(TypeIsValid(Dest)))) {
		return NULL;
	}
	pRunway pNewRunway = NULL;
	if ((pNewRunway = (pRunway)malloc(sizeof(Runway)))) {
		pNewRunway->pflightLinker = NULL;
		pNewRunway->destination = Dest;
		pNewRunway->RunwayNum = runwayNum;
	}
	return pNewRunway;
}

//**************************************************************
//* Function name: destroyRunway
//* Description: delete the runway and all the flight on it 
//* Parameters: pointer to runway.
//* return value: none.
//**************************************************************
void destroyRunway(pRunway prunway) {
	if (prunway == NULL)
		return;  
	pFlighsList tmp = prunway->pflightLinker; //pointer to the first flight on the runway
	pFlighsList NextFlight;
	while (tmp != NULL) {
		NextFlight = tmp->pNextNode;
		destroyFlight(tmp->flightData);
		destroyNodeFlight(tmp);
		tmp = NextFlight;
	}
	free(prunway);
}

//**************************************************************
//* Function name: isFlightExists
//* Description: check if the flight is on the waiting list. 
//* Parameters: pointer to runway, flight number.
//* return value: true is the flight number exists in the list and false else.
//**************************************************************
BOOL isFlightExists(pRunway prunway, int flightNum) {
	if ((prunway == NULL) || (!(DataIsValid(flightNum))))
		return FALSE;
	pFlighsList tmp = prunway->pflightLinker;
	while (tmp != NULL) //check all the linked list
	{
		if (equal(getFlightNumber(tmp->flightData), flightNum)) {
			return TRUE;
		}
		tmp = tmp->pNextNode;
	}
	return FALSE;
}

//**************************************************************
//* Function name: getFlightNum
//* Description: return the number of flights that waiting on the runway. 
//* Parameters: pointer to runway.
//* return value: number of flights waiting to depart.
//**************************************************************
int getFlightNum(pRunway prunway) {
	if (prunway == NULL)
		return -1; 
	int count = 0;
	pFlighsList tmp = prunway->pflightLinker;
	while (tmp != NULL) {
		count++;
		tmp = tmp->pNextNode;
	}
	return count;
}

//**************************************************************
//* Function name: addFlight
//* Description: add a new flight to the right spot on the list. 
//* Parameters: pointer to runway , pointer to flight.
//* return value: success in case we add the flight and failure otherwise.
//**************************************************************
Result addFlight(pRunway prunway, pflightElement pflight) {
	if ((!(fullCheck(prunway, pflight))))
		return FAILURE;

	//set the flight as node
	pFlighsList pNewNode = NULL;
	if (!(pNewNode = (pFlighsList)malloc(sizeof(flighsList)))) 
		return FAILURE;
	pNewNode->flightData = pflight;
	pNewNode->pNextNode = NULL;

	//start to put the node on the right place
	if ((getFlightNum(prunway) == 0)) //there is no flights at all .
	{
		prunway->pflightLinker = pNewNode;
		return SUCCESS;
	}
	pFlighsList tmp = prunway->pflightLinker;

	//we have at list one flight on runway
	if (!(getIfEmergency(pflight)))  //the new flight is not emergency flight
	{ 
		while (tmp->pNextNode != NULL) //move until the last spot and we already know there is at list one flight
		{
			tmp=tmp->pNextNode;
		}
		tmp->pNextNode = pNewNode;
		return SUCCESS;
	}
	else //the new flight is emergency flight
	{
		int emerNum = getEmergencyNum(prunway);
		if (emerNum == 0) //there is no emergency flights at all ,so we need to add in the beginning
		{
			pNewNode->pNextNode = tmp;
			prunway->pflightLinker = pNewNode;
			return SUCCESS;
		}
		else //there is more emergency flights 
		{
			int i;
			for (i = 1; i < emerNum; i++)
			{
				tmp = tmp->pNextNode;
			}

			pNewNode->pNextNode = tmp->pNextNode; //tmp is the last emergency flight
			tmp->pNextNode = pNewNode;
		}
	}
	return SUCCESS;
}

//**************************************************************
//* Function name: removeFlight
//* Description: remove the flight from the list. 
//* Parameters: pointer to runway, flight number to remove.
//* return value: failure or success.
//**************************************************************
Result removeFlight(pRunway prunway, int flightNum) {
	if ((prunway == NULL) || (!(DataIsValid(flightNum)))||(getFlightNum(prunway)==0))
		return FAILURE;
	pFlighsList tmp = prunway->pflightLinker;

	//if first flight sould be remove
	if ((getFlightNumber(tmp->flightData) == flightNum))  
	{
		prunway->pflightLinker = tmp->pNextNode;//set the second flight to be the first
		destroyFlight(tmp->flightData);
		destroyNodeFlight(tmp);
		return SUCCESS;
	}
	//there is a list of flights to search
	else 
	{
		while (tmp->pNextNode != NULL) //check the all list 
		{
			if ((getFlightNumber(tmp->pNextNode->flightData)) == flightNum) //we found that the next flight is the one we search for, its for sure not the first in the list .we get in only for the second flight
			{
				pFlighsList nodeToDel = tmp->pNextNode;
				tmp->pNextNode = nodeToDel->pNextNode;
				destroyFlight(nodeToDel->flightData);
				destroyNodeFlight(nodeToDel);
				return SUCCESS;
			}
			tmp = tmp->pNextNode;
		}
	}
	//if the flight not on that runway
	return FAILURE;
}

//**************************************************************
//* Function name: depart
//* Description: remove the first flight from the waiting list. 
//* Parameters: pointer to runway.
//* return value: failure or success.
//**************************************************************
Result depart(pRunway prunway) {
	if ((prunway == NULL) || (getFlightNum(prunway) == 0))
		return FAILURE;
	pFlighsList tmp = prunway->pflightLinker;
	prunway->pflightLinker = tmp->pNextNode;
	destroyFlight(tmp->flightData); 
	destroyNodeFlight(tmp);
	return SUCCESS;
}

//**************************************************************
//* Function name: printRunway
//* Description: recieves a pointer to runway and print number , type and flights. 
//* Parameters: pointer to runway.
//* return value: none.
//**************************************************************
void printRunway(pRunway prunway) {
	if (prunway == NULL)
		return;
	if (getRunwayType(prunway) == DOMESTIC) {
		printf("Runway %d domestic\n", prunway->RunwayNum);
	}
	else {
		printf("Runway %d international\n", prunway->RunwayNum);
	}
		printf("%d flights are waiting:\n", getFlightNum(prunway));
		pFlighsList tmp = prunway->pflightLinker;
		while (tmp != NULL) {
			printFlight(tmp->flightData);
			tmp = tmp->pNextNode;
		}
}

//**************************************************************
//* Function name: getEmergencyNum
//* Description: recieves a pointer to runway and print number of emergency flights. 
//* Parameters: pointer to runway.
//* return value: number of emergency flights.
//**************************************************************
int getEmergencyNum(pRunway prunway) {
	if (prunway == NULL)
		return -1; 
	int count = 0;
	pFlighsList tmp = prunway->pflightLinker;
	while (tmp!=NULL) //move until the end.
	{
		if(getIfEmergency(tmp->flightData))//count only the emergency
			count++;
		tmp = tmp->pNextNode;
	}
	return count;
}


BOOL DataIsValid(int num) {
	return (num >= 1) && (num <= MAX_ID);
}

BOOL TypeIsValid(FlightType dest) {
	return (dest == DOMESTIC) || (dest == INTERNATIONAL);
}

//check if the numbers are equal
BOOL equal(int flight, int newFlight) {
	return flight == newFlight;
}

//Getters
int getRunwayNum(pRunway prunway) {
	return prunway->RunwayNum;
}
FlightType getRunwayType(pRunway prunway) {
	return prunway->destination;
}

pFlighsList getFlightsLinker(pRunway runwayNode) {
	return runwayNode->pflightLinker;
}

pflightElement getFlightData(pFlighsList flightNode) {
	return flightNode->flightData;
}

pFlighsList getNextFlight(pFlighsList flightNode) {
	return flightNode->pNextNode;
}

//check if we can add the flight to the runway
BOOL fullCheck(pRunway prunway, pflightElement pflight) {
	if ((prunway == NULL) || (isFlightExists(prunway, (getFlightNumber(pflight)))) || (!(sameDest(getRunwayType(prunway), getFlightType(pflight)))) || (!(DataIsValid(getRunwayNum(prunway)))) || (!(DataIsValid(getFlightNumber(pflight))))) {
		return FALSE;
	}
	return TRUE;
}

//check the destination is the same ,the runway and the flight
BOOL sameDest(FlightType runway, FlightType flight) {
	if ((!(TypeIsValid(runway))) || (!(TypeIsValid(flight)))) {
		return FALSE;
	}
	return runway == flight;
}


//**************************************************************
//* Function name: destroyNodeFlight
//* Description: delete the node and all the data on it.
//* Parameters: pointer to node.
//* return value: none.
//**************************************************************




void destroyNodeFlight(pFlighsList pNodeFlight)
{
	if (pNodeFlight == NULL)
		return;
	free(pNodeFlight);
}




#endif 
