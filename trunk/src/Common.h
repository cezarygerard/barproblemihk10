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

//constants:
#define	NUM_TABLES				5
#define	NUM_TABLE_UNITS			10
#define NUM_GLASSES				20
#define NUM_CUPS				10
#define	MAX_DRINKS_PER_CUST		10
#define	RATIO_BEER				0.50
#define	RATIO_CAPPUCCINO		0.30
#define	RATIO_HOT_CHOCOLATE		0.20

#define	TIME_UNTIL_LASTCALL		25000	//milliseconds
#define	TIME_UNTIL_CLOSE		5000	//length of last call in milliseconds

//preparing delays:
//


enum OrderType {beer, cappuccino, hot_chocolate } ;

//int msqid;
pthread_mutex_t beerTap;// = PTHREADINITIALIZE;
pthread_mutex_t cupboard; //??
pthread_mutex_t milk;
pthread_mutex_t coffee;
pthread_mutex_t chocolate;

sem_t glasses;
sem_t tables[NUM_TABLES];

int currentCustID;


//prints formatted text to console
void log(const char *name, const char *message);
void log(const char *message);





#endif /* COMMON_H_ */
