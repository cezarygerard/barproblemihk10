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
	pair<int,int> collectDishes();
	int cleanDishes(int dishes);
	pair<int,int>  replaceDishes(pair<int,int> collectedDishes); //glasses, cups
	void takeBreak() ;

	///mutex and conditional variable used to force thread to timeout on condition_cond using pthread_cond_timedwait
	//pthread_mutex_t count_mutex ;
	//pthread_cond_t  condition_cond ;
	//sem_t final_run_sem;
	pthread_t runThread;
	int greeting_q_id;
	bool bClose;
	void run();
	void doCleanupRound();
	void greetLandlord(bool leaving);

public:
	Assistant(int dqi);
	virtual ~Assistant();
	static void* run_thread(void *);
	void lastOrder();
	void closeUp();
	typedef struct __Assistant_Thread_Args {
			int greeting_q_id;
		} Assistant_Thread_Args;
};

#endif /* ASSISTANT_H_ */
