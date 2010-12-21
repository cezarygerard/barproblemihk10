/*
 * Landlord.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Landlord.h"

Landlord::Landlord(int gqi, int dqi) {
	name = "Landlord";
	greeting_q_id = gqi;
	drink_q_id = dqi;
}

Landlord::~Landlord() {
	// TODO Auto-generated destructor stub
}

void Landlord::run()
{
	ostringstream temp;
	temp << "Landlord got to work.";
	log(name, temp.str());

	//greet and bartend until its time for last call
	while (!bLastCall)
	{
		greet();
		bartend();
	}

	//inform everyone in the bar of last call
	lastCall();

	//continue to greek and bartend until only the barmaid and assistant are left
	while (!numPeopleInBar(2))
	{
		greet();
		bartend();
	}

	//tell assistant and barmaid to close up
	for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
			((*it).second)->closeUp();

	//say goodbye to employees until the bar is empty
	while (!numPeopleInBar(0))
	{
		greet();
	}

	string msg = "Clocking out.";
	log(name, msg);
	pthread_exit(NULL);
}

void Landlord::greet()
{
	ostringstream temp;
	Greeting_Msg_Args msg_buf;
	int len = 1;
	while (len != 0)
	{
		len = msgrcv(greeting_q_id, (void*) &msg_buf, sizeof(Greeting_Msg_Args), 0,IPC_NOWAIT);
		if (len == -1)
		{
			if(errno == ENOMSG )
				return;
			temp.str("");
			temp << "error greeting - " << errno << "  q_id: " << greeting_q_id;
			log(name, temp.str());
			usleep(250000);
		}

		temp.str("");
		if (!msg_buf.leaving)
		{
			//add person to map
			people_in_bar[msg_buf.person_id] = msg_buf.person_ptr;
		}
		else
		{
			//remove person from map
			people_in_bar.erase(msg_buf.person_id);
		}

		temp.str("");
		temp << "cust_id of people in the bar:";
		for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
			temp << " " << (*it).first;

		log(name, temp.str());
	}
}

void Landlord::lastCall()
{
	//call everyone's lastOrder() method
	for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
		((*it).second)->lastOrder();
}

bool Landlord::numPeopleInBar(int n)
{
	int i = (int)people_in_bar.size();
	if (i == n)
	{
		return true;
	}

	return false;
}


void* Landlord::run_thread(void *dptr)
{
	Landlord_Thread_Args *cptr = (Landlord_Thread_Args *) dptr;
	Landlord * ll = new Landlord(cptr->greeting_q_id, cptr->drink_q_id);
	ll->run();
	return 0;
}
