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

	///number of drinks left
	int drinksLeft;
	///favourite table
	int favTableIndex;
	///orders queue id
	int drink_q_id;
	///greetings to landlord queue id
	int greeting_q_id;

	///chooses favorite drink in random manner
	///@return randomly chosen drink type
	static OrderType chooseDrink();

	///chooses number of drinks to drink in random manner
	///@return randomly chosen maximum number of drinks
	static int chooseMaxDrinks();

	///chooses favorite table in random manner
	///@param randomly chosen favorite table index
	static int chooseFavTable();

	///sends message to landlord to register / deregister customer
	///@param leaving	true if leaving, false if greeting
	void greetLandlord(bool leaving);

	///sends drink order to the drink queue
	void orderDrink();

	///sleeps for set amount of time to drink chosen beverage
	///if beer is chosen, thread will wait instead of sleep, which can woken with a signal
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
	Customer(int cid, int dqi, int gqi);
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

	///greets landlord and runs the main loop for customer actions in the bar
	///calls orderDrink() and drink() until customer is done. Then it greets landlord saying goodbye.
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
