/*
 * Table.h
 *	Class dealing with tables: putting and collecting cups and glasses
 *
 *		Class does not cause waiting while putting/collecting glasses
 *  Created on: Dec 11, 2010
 *      Author: czarek
 *
 */

#ifndef TABLE_H_
#define TABLE_H_

#include "Common.h"

class Table {
	///mutex for dish counts
	pthread_mutex_t dishMutex_;
	///semaphore representing empty units on table
	sem_t emptyUnits_;
	///number of glasses on table
	int glasses_;
	///number of cups on table
	int cups_;

public:

	///Should be run by customer: puts dish on the table
	///@param GLASS / CUP to be put on table
	void putDish(DishType dt);

	///Should be run by assistant: removes dish from table
	///@param dt - place to write what DishType was collected
	///@return value indicating if anything was collected (value 1 means that table was empty)
	int collectNextDish(DishType& dt);

	Table();
	virtual ~Table();
};

#endif /* TABLE_H_ */

