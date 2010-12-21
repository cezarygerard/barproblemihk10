/*
 * Landlord.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */
#ifndef LANDLORD_H_
#define LANDLORD_H_

#include <map>
#include "BarEmp.h"

class Landlord: public BarEmp{
private:

	///message queue id to greet/say goodbye to landlord
	int greeting_q_id;

	///map of people in the bar
	///first = id, second = pointer to the person
	std::map<int, Person*> people_in_bar;

	///greet everyone currently in the message queue
	void greet();

	///inform everyone in the bar that it's last call by calling their lastOrder() method
	void lastCall();

	///returns true if the number of people in the bar match the given number
	///@param n	given number of people
	///@return	true if the number matchs, false if not
	bool numPeopleInBar(int n);

public:

	///@param gqi	greeting queue id
	///@param dqi	drink queue id
	Landlord(int gqi, int dqi);
	virtual ~Landlord();

	///continuously greets people and bartends until everyone has left bar
	///also informs everyone of last call
	void run();

	///creates new customer, and calls run().
	///this function is specified when new pthread_create is call in main whenever new customer has to be created
	static void* run_thread(void *dptr);

	//this is used as argument for run_thread in pthread_create
	typedef struct __Landlord_Thread_Args {
		int greeting_q_id;
		int drink_q_id;
	} Landlord_Thread_Args;

	//this is used as and agrument for greeting and saying goodbye to the Landlord
	typedef struct __Greeting_Msg_Args {
		bool leaving;
		//id's weren't passing using int, long worked better
		long person_id;
		Person* person_ptr;
	} Greeting_Msg_Args;
};

#endif /* LANDLORD_H_ */
