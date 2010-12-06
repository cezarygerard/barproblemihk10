/*
 * common.h
 *
 *  Created on: Dec 3, 2010
 *      Author: czarek
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <string>

using namespace std;

//constants:
#define	NUM_TABLES				5
#define	NUM_TABLE_UNITS			10
#define NUM_GLASSES				20
#define NUM_CUPS				10

#define	MAX_DRINKS_PER_CUST		10		//will choose rand number from 1 to this const for each cust

#define	RATIO_BEER				0.50
#define	RATIO_CAPPUCCINO		0.30
#define	RATIO_HOT_CHOCOLATE		0.20

#define	TIME_UNTIL_LASTCALL		25000	//milliseconds
#define	TIME_UNTIL_CLOSE		5000	//length of last call in milliseconds
#define TIME_INTERVAL_CUST		200		//how often a customer enters the bar
#define TIME_TO_DRINK			50		//how long it takes for a customer to consume any drink

#define TIME_GET_DISH			5
#define	TIME_FILL_GLASS			10
#define	TIME_GET_MILK			5
#define	TIME_GET_COFFEE			5
#define	TIME_GET_CHOCOLATE		5
#define TIME_MIX_DRINK			10

#define TIME_COLLECT_DISH		1
#define	TIME_WASH_DISH			10
#define TIME_REPLACE_DISH		2
#define TIME_REST				100


enum OrderType {BEER, CAPPUCCINO, HOT_CHOCOLATE };
const char* typeAsString(OrderType type);

extern int msqid, currentCustID;
extern pthread_mutex_t beerTap, cupboard, milk, coffee, chocolate;
extern sem_t glasses, cups, tables[NUM_TABLES];


//prints formatted text to console
void log(string &name, string &message);
void log(string &message);


#endif /* COMMON_H_ */
