/*
 * common.h
 *
 *  Created on: Dec 3, 2010
 *      Author: czarek
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <assert.h>
//constant variables:
const int numberOfTables;
const int numberOfSpacecOnTable;
const int numberOfGlasses;
const int numberOfCups;
const int numberOfDrinksPerCustomer;
const double beerRatio;
const double capuchinnoRatio;
const double chocolateRatio;
const int timeUntilClose;
const int timeUntilLastCall;
//preparing delays:
//


enum OrderType {beer, cappuccino, chocolate } ;
int msqid;
pthread_mutex_t beerTap;// = PTHREADINITIALIZE;
pthread_mutex_t cupBoard; //??
pthread_mutex_t milk;
pthread_mutex_t coffe;
pthread_mutex_t chocolate;
pthread_mutex_t beerTap;

sem_t glasses;
sem_t tables[numberOfTables];

//let's make this the init function for pthreads to call
void run();


#endif /* COMMON_H_ */
