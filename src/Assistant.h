/*
 * Assistant.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef ASSISTANT_H_
#define ASSISTANT_H_

#include "Person.h"
#include "Table.h"

class Assistant: public Person{

	///Assistant makes round collecting dishes
	///@return number of dishes collected (Glasses, cups) subsequently
	pair<int,int> collectDishes();

	///cleans collected dishes
	///@param dishes number of dishes to clean
	///@return number of dishes cleaned
	int cleanDishes(int dishes);

	///replaces dishes increasing semaphores vaules
	///@param collectedDishes <glasses, cups> to replace
	///@return the <glasses, cups> that were replaced
	pair<int,int>  replaceDishes(pair<int,int> collectedDishes); //glasses, cups

	///sleeps the current thread for the time specified in common
	void takeBreak() ;

	///mutex and conditional variable used to force thread to timeout on condition_cond using pthread_cond_timedwait
	//pthread_mutex_t count_mutex ;
	//pthread_cond_t  condition_cond ;
	//sem_t final_run_sem;
	///
	pthread_t runThread;

	///message queue id to greet/say goodbye to landlord
	int greeting_q_id;

	///true if bar is closed
	bool bClose;

	///calls cleanDishes and replaceDishes
	void doCleanupRound();

	///main loop for assistant, calls mainly doCleanupRound() and takeBreak(), plus greetings and final round
	void run();

	///greets/says goodbye to landlor
	///@param leaving	true if leaving, false if greeting
	void greetLandlord(bool leaving);

public:
	///@param greetings queue id
	Assistant(int gqi);
	virtual ~Assistant();

	///creates new assistant, and calls run().
	///this function is specified when new pthread_create is called new assistant has to be created
	static void* run_thread(void *);

	//placeholder for virtual method
	void lastOrder();

	///informs assistant that bar is already closed
	void closeUp();

	//this is used as argument for run_thread in pthread_create
	typedef struct __Assistant_Thread_Args {
			///greetings queue id
			int greeting_q_id;
		} Assistant_Thread_Args;
};

#endif /* ASSISTANT_H_ */
