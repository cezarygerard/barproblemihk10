/*
 * Table.cpp
 *
 *  Created on: Dec 11, 2010
 *      Author: czarek
 */

#include "Table.h"
#include <cassert>

Table::Table() {
	// TODO Auto-generated constructor stub
	sem_init(&emptyUnits_, 0 , NUM_TABLE_UNITS);
	pthread_mutex_init(&dishMutex_, 0);
	glasses_ =0;
	cups_ = 0;
}

Table::~Table() {
	// TODO Auto-generated destructor stub
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
	assert(glasses_+ cups_ == val&& val < NUM_TABLE_UNITS);
	pthread_mutex_unlock( &dishMutex_ );
}

int Table::collectNextDish(DishType& dt)
{
	int toReturn;
	pthread_mutex_lock( &dishMutex_ );
	if(glasses_ >0)
	{
		dt = GLASS;
		glasses_--;
		toReturn = 0;
	}
	else if(cups_ >0)
	{
		dt = CUP;
		cups_--;
		toReturn = 0;
	}
	else
		toReturn =1;
	pthread_mutex_unlock( &dishMutex_ );
	sem_post(&emptyUnits_);
	return toReturn;
}
