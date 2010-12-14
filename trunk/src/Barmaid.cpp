/*
 * Barmaid.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Barmaid.h"

Barmaid::Barmaid(int dqi) {
	name = "Barmaid";
	drink_q_id = dqi;
}

Barmaid::~Barmaid() {
}

void* Barmaid::run_thread(void *dptr)
{
	Barmaid_Thread_Args *cptr = (Barmaid_Thread_Args *) dptr;
	Barmaid * bm = new Barmaid(cptr->drink_q_id);
	bm->run();
	return 0;
}

void Barmaid::run()
{
	ostringstream temp;
	temp << "Barmaid got to work.";
	string msg = temp.str();
	log(name, msg);

	while(!bClose)
	{
		bartend();
	}

	msg = "Clocking out.";
	log(name, msg);
	pthread_exit(NULL);
}
