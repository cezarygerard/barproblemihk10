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
	int greeting_q_id;
	bool bClose;
	void greetLandlord(bool leaving);
public:
	Barmaid(int dqi, int gqi);
	virtual ~Barmaid();
	static void* run_thread(void *);
	void lastOrder();
	void closeUp();
	typedef struct __Barmaid_Thread_Args {
			int drink_q_id;
			int greeting_q_id;
		} Barmaid_Thread_Args;
};

#endif /* BARMAID_H_ */
