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
	void run();
public:
	Barmaid(int dqi);
	virtual ~Barmaid();
	static void* run_thread(void *);
	typedef struct __Barmaid_Thread_Args {
			int drink_q_id;
		} Barmaid_Thread_Args;
};

#endif /* BARMAID_H_ */
