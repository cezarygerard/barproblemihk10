/*
 * Customer.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Person.h"
#include "Common.h"
#include <pthread.h>
#include <sys/time.h>
#include <pthread.h>

class Customer: public Person {
	OrderType orderType;
	int drinksLeft;
	int favTableIndex;

	OrderType chooseDrink();
	int chooseMaxDrinks();
	int chooseFavTable();
	static void* threadFun(void * landllord);
	void start();
	void orderDrink();
	void drink();
	pthread_mutex_t mutex;
	pthread_cond_t  condition;
	///time structures:
	timeval tp;
	timespec ts;

public:
	Customer();
	virtual ~Customer();
	static void run(void* landlord);
	void lastOrder();
};

#endif /* CUSTOMER_H_ */
