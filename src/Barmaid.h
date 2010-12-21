/*
 * Barmaid.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef BARMAID_H_
#define BARMAID_H_

#include "BarEmp.h"
#include "Common.h"

class Barmaid: public BarEmp {

private:
	///main run method for Barmaid, bartends until bClose is true
	void run();

	///message queue id to greet/say goodbye to landlord
	int greeting_q_id;

	///true if bar is closed
	bool bClose;

	///greets/says goodbye to landlord
	///@param leaving	true if leaving, false if greeting
	void greetLandlord(bool leaving);

public:
	///@param dqi drink queue id
	///@param gqi greeting queue id
	Barmaid(int dqi, int gqi);
	virtual ~Barmaid();

	///creates a new Barmaid and calls run()
	static void* run_thread(void *);

	///placeholder for virtual method
	void lastOrder();

	///change bClose to true
	void closeUp();

	//this is used as argument for run_thread in pthread_create
	typedef struct __Barmaid_Thread_Args {
			int drink_q_id;
			int greeting_q_id;
		} Barmaid_Thread_Args;
};

#endif /* BARMAID_H_ */
