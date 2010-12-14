/*
 * Customer.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <pthread.h>
#include <sys/time.h>
#include "Person.h"

class Customer: public Person {
	OrderType orderType;
	int drinksLeft;
	int favTableIndex;
	//int my_q_id;

	int drink_q_id;
	int greeting_q_id;

	static OrderType chooseDrink();
	static int chooseMaxDrinks();
	static int chooseFavTable();
	void greetLandlord(bool leaving);
	static void* threadFun(void * landllord);
	void start();
	void orderDrink();
	void drink();
	void chillAtPub();
	pthread_mutex_t mutex;
	pthread_cond_t lc_condition;
	pthread_cond_t rdy_condition;
	///time structures:
	timeval tp;
	timespec ts;

public:
	Customer(int cid, /*int cmqi,*/ int dqi, int gqi);
	virtual ~Customer();
	void run();
	void lastOrder();
	void receiveDrink();
	int my_id;
	typedef struct __Cust_Thread_Args {
		int	cust_id;
		//int cust_msg_q_id;
		int drink_q_id;
		int greeting_q_id;
	} Cust_Thread_Args;
	static void *run_thread(void *dptr);
};

#endif /* CUSTOMER_H_ */
