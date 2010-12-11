/*
 * Assistant.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef ASSISTANT_H_
#define ASSISTANT_H_

#include <pthread.h>
#include "Person.h"
#include <sys/time.h>

class Assistant: public Person{
	pair<int,int> collectDishes();
	int cleanDishes(int dishes);
	pair<int,int>  replaceDishes(pair<int,int> collectedDishes); //glasses, cups
	void takeBreak() ;

	///mutex and conditional variable used to force thread to timeout on condition_cond using pthread_cond_timedwait
	pthread_mutex_t count_mutex ;
	pthread_cond_t  condition_cond ;

	pthread_t runThread;

	bool timeToFinish;

	///time structures:
	timeval tp;
	timespec ts;

	///useful with converting time structures:
	static const long long msToNs = 1000 * 1000;

	///useful with converting time structures:
	static const long long nsToS= 1000 * 1000 * 1000;

	void doStuff();

	///final cleaning after all customers have left
	///should be called by landlord
	void doFinalRun();

	///One cleaning go
	void doCleanupRound();

//	static void thread_fun() ;

public:
	Assistant();



	///infinite loop, should be thread's method
	void run();

	virtual ~Assistant();
};

#endif /* ASSISTANT_H_ */
