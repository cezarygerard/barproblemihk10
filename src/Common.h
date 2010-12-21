/*
 * common.h
 *
 *  Created on: Dec 3, 2010
 *      Author: czarek
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <iostream>
#include <sstream>
#include <list>
//#include "Table.h"

using namespace std;

class Table;

//msg queue declarations:
#define DRINK_Q 0x4
#define GREET_Q 0x5
//#define CUSTOMER_START_Q 0x20


//constants:

///number of limited resources
#define	NUM_TABLES				5
#define	NUM_TABLE_UNITS			10
#define NUM_GLASSES				20
#define NUM_CUPS				10

//will choose rand number from 1 to this const for each cust
#define	MAX_DRINKS_PER_CUST		20

///order ratio for certain drinks, summing up must be ==1 (otherwise assertion will fail)
#define	RATIO_BEER				0.50
#define	RATIO_CAPPUCCINO		0.30
#define	RATIO_HOT_CHOCOLATE		0.20

//
#define	TIME_UNTIL_LASTCALL		5000	//milliseconds
//#define	TIME_UNTIL_CLOSE		5000	//length otf last call in milliseconds
#define TIME_INTERVAL_CUST		200		//how often a customer enters the bar
#define TIME_TO_DRINK			50		//how long it takes for a customer to consume any drink

///time definitions fo assistant
#define TIME_GET_DISH			5
#define	TIME_FILL_GLASS			10
#define	TIME_GET_MILK			5
#define	TIME_GET_COFFEE			5
#define	TIME_GET_CHOCOLATE		5
#define TIME_MIX_DRINK			10

///time definisions for assistant
#define TIME_COLLECT_DISH		1
#define	 TIME_WASH_DISH			10
#define TIME_REPLACE_DISH		2
#define TIME_REST				200

///types of drinks to order
enum OrderType {BEER, CAPPUCCINO, HOT_CHOCOLATE };

///Types of dishes
enum DishType {CUP, GLASS};

///types of messages to landlord (in order to register / deregister person)
enum GreetingEventType{ ENTERING, LEAVING };

///converts OrderType to string
const char* typeAsString(OrderType type);

//extern int msqid;
extern pthread_mutex_t beerTap, cupboard, milk, coffee, chocolate;
/// semaphores abstracting glasses and cups
extern sem_t glasses, cups;//, final_run_sem , tables[NUM_TABLES];
//extern sem_t assistantFinalRun, landLordExit;
///table of semaphores abstracting tables, each table has limited NUM_TABLE_UNITS defined before
extern Table tables[NUM_TABLES];


extern volatile bool bLastCall, bClose;

//prints formatted text to console
void log(string name, string message);
void log(string message);

///function which runs the clock
void* run_clock(void *dptr);

/// returns thread-safe random number
int getRand();

///constants useful for converting different time formats
const long long msToNs = 1000 * 1000;
const long long nsToS= 1000 * 1000 * 1000;

#endif /* COMMON_H_ */
