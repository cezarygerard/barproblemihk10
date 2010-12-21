/*
 * Table.cpp
 *
 *  Created on: Dec 11, 2010
 *      Author: czarek
 */

#include "Table.h"
#include <cassert>

Table::Table() {
	sem_init(&emptyUnits_, 0 , NUM_TABLE_UNITS);
	pthread_mutex_init(&dishMutex_, 0);
	glasses_ =0;
	cups_ = 0;
}

Table::~Table() {
}


void Table::putDish(DishType dt)
{

//	resource availability is checked in
//	sem_wait(&emptyUnits_);
//	if there is no space on table thread waits here, before
//	pthread_mutex_lock( &dishMutex_ );
//
//	sem_getvalue(&emptyUnits_, &val);
//	is used to acquire the value of semaphore into "val"
//	only for the assertion beneath.
	//Update: assertion removed

	sem_wait(&emptyUnits_);

	pthread_mutex_lock( &dishMutex_ );

	int val;
	sem_getvalue(&emptyUnits_, &val);

	switch(dt)
	{
	case GLASS:
		glasses_++;
		break;
	case CUP:
		cups_++;
		break;
	}

	pthread_mutex_unlock( &dishMutex_ );
}

int Table::collectNextDish(DishType& dt)
{
	//collect the next dish by decrementing the num of glasses or cups on the table
	//also post to the emptyUnits semaphore

	int toReturn;
	pthread_mutex_lock( &dishMutex_ );
	if(glasses_ >0)
	{
		dt = GLASS;
		glasses_--;
		toReturn = 0;
		sem_post(&emptyUnits_);
	}
	else if(cups_ >0)
	{
		dt = CUP;
		cups_--;
		toReturn = 0;
		sem_post(&emptyUnits_);
	}
	else
		toReturn =1;
	int val;
	sem_getvalue(&emptyUnits_, &val);
	pthread_mutex_unlock( &dishMutex_ );

	return toReturn;
}
