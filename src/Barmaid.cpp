/*
 * Barmaid.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Barmaid.h"
#include "Landlord.h"

Barmaid::Barmaid(int dqi, int gqi) {
	name = "Barmaid";
	drink_q_id = dqi;
	greeting_q_id = gqi;
	bClose = false;
}

Barmaid::~Barmaid() {
}

void* Barmaid::run_thread(void *dptr)
{
	Barmaid_Thread_Args *cptr = (Barmaid_Thread_Args *) dptr;
	Barmaid * bm = new Barmaid(cptr->drink_q_id, cptr->greeting_q_id);
	bm->run();
	return 0;
}

void Barmaid::run()
{
	ostringstream temp;
	temp << "Barmaid got to work.";
	string msg = temp.str();
	log(name, msg);
	//greet landlord
	greetLandlord(false);

	//bartend until closing time
	while(!bClose)
	{
		bartend();
	}

	msg = "Clocking out.";
	log(name, msg);
	greetLandlord(true);
	pthread_exit(NULL);
}

void Barmaid::greetLandlord(bool leaving){
	Landlord::Greeting_Msg_Args tosend;
	tosend.leaving = leaving;
	tosend.person_id = -2;
	tosend.person_ptr = this;

	string msg = "";
	if(leaving)
		msg = "Waiting to say goodbye to landlord...";
	else
		msg = "Waiting for greeting from landlord...";
	log(name, msg);

	int val = msgsnd(greeting_q_id, (void*)&tosend, sizeof(Landlord::Greeting_Msg_Args), 0);
	if(val == -1) {
		msg = "ERROR: Failed to send greeting message";
		log(name, msg);
		if(errno == EACCES)
		{
			msg = "ERROR: Please run program with elevated permissions.";
			log(msg);
			exit(0);
		}
	}
	else if (!leaving)
		msg = "Time to serve some drinks!";
	else
		msg = "See you tomorrow!";
	log(name, msg);
}

void Barmaid::lastOrder()
{
	//placeholder for virtual method
}

void Barmaid::closeUp()
{
	bClose = true;
}
