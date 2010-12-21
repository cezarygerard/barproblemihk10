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
	///type of drink which this customers orders
	OrderType orderType;

	int drinksLeft;

	///favourite table

	int favTableIndex;
	//int my_q_id;

	///orders queue id
	int drink_q_id;

	///greetings to landlord queue id
	int greeting_q_id;

	///chooses favorite drink in random manner
	static OrderType chooseDrink();

	///chooses number of drinks to drink in random manner
	static int chooseMaxDrinks();

	///chooses favorite table in random manner
	static int chooseFavTable();

	///sends message to landlord to register / deregister customer
	void greetLandlord(bool leaving);

	//
	//static void* threadFun(void * landllord);
	//void start();

	///main loop for customer actions int he pub
	///calles orderDrink() and drink() until customer is done. Then it greets landlord saying goodbye.
	void chillAtPub();


	void orderDrink();
	void drink();



	///this conditional variable is used fo pthread_cond_timedwait when customer is drinking beer
	///when last order occurs this variable is signaled and customer goes for new drink
	pthread_cond_t lc_condition;

	///ready drink conditional variable
	///after ordering drink customer is blocked on this conditional variable
	///until drink is prepared by landlord/barmain
	pthread_cond_t rdy_condition;

	///mutex use along with mentioned condional variables
	pthread_mutex_t mutex;

	///time structures:
	timeval tp;
	timespec ts;

public:
	///Customer constructor, called from run_thread
	///@param cid customer id
	///@param diq message queue id for ordering drinks
	///@param cid message queue id for greeting landlord
	Customer(int cid, /*int cmqi,*/ int dqi, int gqi);
	virtual ~Customer();


	///informs customer about last orders. Here beer drinkers are interrupted
	void lastOrder();

	///informes customer that his recently ordered drink is ready
	void receiveDrink();

	///Customer id number
	int my_id;

	///creates new customer, and calls run().
	///this function is specified when new pthread_create is call in main whenever new customer has to be created
	static void *run_thread(void *dptr);

	//greets landlord and calls chillAtPub()
	void run();

	//this is used as argument for run_thread in pthread_create
	typedef struct __Cust_Thread_Args {
		/// cid customer id
		int	cust_id;
		//int cust_msg_q_id;
		/// diq message queue id for ordering drinks
		int drink_q_id;
		/// message queue id for greeting landlord
		int greeting_q_id;
	} Cust_Thread_Args;


};

#endif /* CUSTOMER_H_ */
