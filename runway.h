#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "ex2.h"
#ifndef _runway_H_
#define _runway_H_

typedef struct FlightsList *pFlighsList;
typedef struct NewRunway *pRunway;



//**************************************************************
//* Function name: createRunway
//* Description: creates a new Runway with the needed information.
//* Parameters: Runway number, Runway type.
//* return value: a pointer to the new Runway or NULL in case the function failed.
//**************************************************************

pRunway createRunway(int runwayNum , FlightType dest);

//**************************************************************
//* Function name: destroyRunway
//* Description: delete the runway and all the flight on it 
//* Parameters: pointer to runway.
//* return value: none.
//**************************************************************

void destroyRunway(pRunway prunway);

//**************************************************************
//* Function name: destroyNodeFlight
//* Description: delete the node and all the data on it.
//* Parameters: pointer to node.
//* return value: none.
//**************************************************************

void destroyNodeFlight(pFlighsList pNodeFlight);

//**************************************************************
//* Function name: printRunway
//* Description: recieves a pointer to runway and print number , type and flights. 
//* Parameters: pointer to runway.
//* return value: none.
//**************************************************************

void printRunway(pRunway prunway);

//**************************************************************
//* Function name: isFlightExists
//* Description: check if the flight is on the waiting list. 
//* Parameters: pointer to runway, flight number.
//* return value: true is the flight number exists in the list and false else.
//**************************************************************

BOOL isFlightExists(pRunway prunway, int flightNum);

//**************************************************************
//* Function name: getEmergencyNum
//* Description: recieves a pointer to runway and print number of emergency flights. 
//* Parameters: pointer to runway.
//* return value: number of emergency flights.
//**************************************************************

int getEmergencyNum(pRunway prunway);

//getters
int getFlightNum(pRunway prunway);
int getRunwayNum(pRunway prunway);
FlightType getRunwayType(pRunway prunway);
pFlighsList getFlightsLinker(pRunway runwayNode);
pFlighsList getNextFlight(pFlighsList flightNode);
pflightElement getFlightData(pFlighsList flightNode);


//setters

//**************************************************************
//* Function name: addFlight
//* Description: add a new flight to the right spot on the list. 
//* Parameters: pointer to runway , pointer to flight.
//* return value: success in case we add the flight and failure otherwise.
//**************************************************************

Result addFlight(pRunway prunway, pflightElement pflight);

//**************************************************************
//* Function name: removeFlight
//* Description: remove the flight from the list. 
//* Parameters: pointer to runway, flight number to remove.
//* return value: failure or success.
//**************************************************************

Result removeFlight(pRunway prunway, int flightNum);

//**************************************************************
//* Function name: depart
//* Description: remove the first flight from the waiting list. 
//* Parameters: pointer to runway.
//* return value: failure or success.
//**************************************************************

Result depart(pRunway prunway);

//other
BOOL DataIsValid(int num);
BOOL TypeIsValid(FlightType dest);
BOOL equal(int flight, int newFlight);
BOOL sameDest(FlightType runway, FlightType flight);
BOOL fullCheck(pRunway prunway, pflightElement pflight);


#endif 
