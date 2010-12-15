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

	while (!bLastCall)
	{
		greet();
		bartend();
	}

	lastCall();

	while (!numPeopleInBar(2))
	{
		greet();
		bartend();
	}

	for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
			((*it).second)->closeUp();

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
			//temp << "Recieved entering message from cust_" << msg_buf.person_id;
			people_in_bar[msg_buf.person_id] = msg_buf.person_ptr;
		}
		else
		{
			//temp << "Recieved leaving message from cust_" << msg_buf.person_id;
			people_in_bar.erase(msg_buf.person_id);
		}

		//log(name, temp.str());
		temp.str("");
		temp << "cust_id of people in the bar:";
		for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
			temp << " " << (*it).first;

		log(name, temp.str());
	}
}

void Landlord::lastCall()
{
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
