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

//	ostringstream str;
//	string temp;
//	str << "LLcons: gqi: " << greeting_q_id << "  dqi: " << drink_q_id;
//	temp = str.str();
//	log(temp);
}

Landlord::~Landlord() {
	// TODO Auto-generated destructor stub
}

void Landlord::run()
{
	ostringstream temp;
	temp << "Landlord got to work.";
	//string msg = temp.str();
	//log(name, msg);
	log(name, temp.str());
	while (!bLastCall)
	{
		greet();

		bartend();
	}
	checkLastCall();
	while (!bClose)
	{
		greet();
		checkClose();
		bartend();
	}

	sem_wait(&landLordExit);
	//wait for employees to leave
	//Assistant and Barmaid should terminate when bClose is true
	//usleep(5000);

	string msg = "Clocking out.";
	log(name, msg);
	pthread_exit(NULL);
}

void Landlord::greet()
{
	ostringstream temp;
//	string msg;
	Greeting_Msg_Args msg_buf;
	int len = 1;
	while (len != 0)
	{
	//log(name);
	len = msgrcv(greeting_q_id, (void*) &msg_buf, sizeof(Greeting_Msg_Args), 0,IPC_NOWAIT);
	//log(name);
	if(len == -1)
	{
		if(errno == ENOMSG )
			return;
		temp.str("");
		temp << "error greeting - " << errno << "  q_id: " << greeting_q_id;
		//msg = temp.str();
		//log(name, msg);
		log(name, temp.str());
		usleep(250000);
	}
	temp.str("");
	if(!msg_buf.leaving)
	{
		temp << "Recieved entering message from cust_" << msg_buf.person_id;
		//people_in_the_bar.push_back(msg_buf.q_id);
		people_in_bar[msg_buf.person_id] = msg_buf.person_ptr;
	}
	else
	{
		temp << "Recieved leaving message from cust_" << msg_buf.person_id;
		//people_in_the_bar.remove(msg_buf.q_id);
		people_in_bar.erase(msg_buf.person_id);
	}
	//msg = temp.str();
	//log(name, msg);
	log(name, temp.str());
	temp.str("");
	temp << "cust_id of people in the bar:";
	//for (list<int>::iterator it=people_in_the_bar.begin() ; it != people_in_the_bar.end(); it++ )
	for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
		temp << " " << (*it).first;
	//msg = temp.str();
	//log(name, msg);
	log(name, temp.str());
	}
	int i = 5;
}

void Landlord::checkLastCall()
{
	if (bLastCall)
	{
		for (map<int, Person*>::iterator it=people_in_bar.begin() ; it != people_in_bar.end(); it++ )
			((Customer*)((*it).second))->lastOrder();
	}
}

void Landlord::checkClose()
{
	int i = (int)people_in_bar.size();
	if (i == 0)
	{
		bClose = true;
		sem_post(&assistantFinalRun);
	}
}


void* Landlord::run_thread(void *dptr)
{
	Landlord_Thread_Args *cptr = (Landlord_Thread_Args *) dptr;
	Landlord * ll = new Landlord(cptr->greeting_q_id, cptr->drink_q_id);
	ll->run();
	return 0;
}
