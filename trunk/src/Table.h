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

#include <pthread.h>
#include "Common.h"

class Table {
	pthread_mutex_t dishMutex_;
	sem_t emptyUnits_;
	int glasses_;
	int cups_;

public:

	/**
	 * should be run by customer
	 * @param GLASS / CUP to be put on table
	 */
	void putDish(DishType dt);

	/**
	 * should be run by assistant
	 * @param dt - place to write what DishType was collected
	 * @return value indicating if anything was collected (value 1 means that table was empty)
	 */
	int collectNextDish(DishType& dt);

	Table();
	virtual ~Table();

};

#endif /* TABLE_H_ */

