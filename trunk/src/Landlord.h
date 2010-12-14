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
	int greeting_q_id;
	//list<int> people_in_the_bar;
	std::map<int, Person*> people_in_bar;
	void greet();
	void checkLastCall();
public:
	Landlord(int gqi, int dqi);
	virtual ~Landlord();
	void run();
	static void* run_thread(void *dptr);
	typedef struct __Landlord_Thread_Args {
		int greeting_q_id;
		int drink_q_id;
	} Landlord_Thread_Args;
	typedef struct __Greeting_Msg_Args {
		bool leaving;
		//id's weren't passing using int...
		long person_id;
		Person* person_ptr;
	} Greeting_Msg_Args;
};

#endif /* LANDLORD_H_ */
