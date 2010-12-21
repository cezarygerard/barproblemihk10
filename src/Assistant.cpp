/*
 * Assistant.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Assistant.h"
#include "Landlord.h"
#include <pthread.h>

Assistant::Assistant(int gqi){
	name = "Assistant";
	greeting_q_id = gqi;
	bClose = false;
}

Assistant::~Assistant() {

}

void* Assistant::run_thread(void * dptr)
{
	Assistant_Thread_Args *cptr = (Assistant_Thread_Args *) dptr;
	Assistant* ass = new Assistant(cptr->greeting_q_id);

	ass->run();
	return 0;
}

void Assistant::run()
{
	ostringstream temp;
	temp << "Assistant got to work.";
	log(name, temp.str());
	//greet the landlord
	greetLandlord(false);

	//clean and rest loop
	while(!bClose)
	{
		takeBreak();
		doCleanupRound();
	}

	//final cleanup
	doCleanupRound();
	string msg = "Clocking out.";
	log(name, msg);
	greetLandlord(true);
	pthread_exit(NULL);
}

void Assistant::greetLandlord(bool leaving){
	Landlord::Greeting_Msg_Args tosend;
	tosend.leaving = leaving;
	//static id for assistant
	tosend.person_id = -1;
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
		msg = "Another hard day...";
	else
		msg = "I'm outta here!";
	log(name, msg);
}

void Assistant::lastOrder()
{
	//placeholder for virtual method
}

void Assistant::closeUp()
{
	bClose = true;
}

void Assistant::doCleanupRound()
{
	string s ="starting cleanup";
	log(name,s);
	pair<int,int> collected = collectDishes();
	int cleaned = cleanDishes(collected.first+collected.second);
	pair<int,int> replaced = replaceDishes(collected);

	ostringstream temp;
	temp << " glasses collected: " << collected.first
			<< " cups collected: " << collected.second
			<< "dishes cleaned: " <<cleaned
			<< " glasses replaced: " << replaced.first
			<< " cups replaced: " << replaced.second;
	log(name, temp.str());
}

std::pair<int,int> Assistant::collectDishes()
{
	int totalCollected = 0;
	//first = glasses, second = cups
	pair<int,int> collectedDishes = pair<int,int>(0,0);

	for (int i = 0; i < NUM_TABLES; i++)
	{
		int this_tbl_units=0;
		DishType collectedDish;
		//collect all dishes on current table
		while(!(tables[i].collectNextDish(collectedDish)))
		{
			totalCollected++;
			this_tbl_units++;
			switch (collectedDish) {
			case GLASS:
				collectedDishes.first++;
				break;
			case CUP:
				collectedDishes.second++;
				break;
			default:
				break;
			}
			usleep(TIME_COLLECT_DISH *1000);
		}
		ostringstream temp;
		temp << "table: "<< i << " dishes collected: " << this_tbl_units;
		log(name, temp.str());

	}
	assert(totalCollected == collectedDishes.first + collectedDishes.second);
	return collectedDishes;
}

std::pair<int,int> Assistant::replaceDishes(pair<int,int> collectedDishes)
{
	//increases glasses semaphore for each glass collected
	for (int j = 0; j< collectedDishes.first ; j++)
	{
		sem_post(&glasses);
		usleep(TIME_REPLACE_DISH *1000);
	}

	//increases cups semaphore for each cup collected
	for (int j = 0; j< collectedDishes.second ; j++)
	{
		sem_post(&cups);
		usleep(TIME_REPLACE_DISH *1000);
	}

	return collectedDishes;
}

int Assistant::cleanDishes(int dishes)
{
	int cleanedDishes = 0; //artificial value, should always equal "dishes"

	for (int j = 0; j< dishes ; j++)
	{
		cleanedDishes++;
		usleep(TIME_WASH_DISH *1000);
	}

	return cleanedDishes;
}

void Assistant::takeBreak()
{
	usleep(TIME_REST * 1000);
}

