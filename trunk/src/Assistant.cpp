/*
 * Assistant.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Assistant.h"

Assistant::Assistant(){
	name = "Assistant";
	timeToFinish = false;
//	pthread_mutex_init(&count_mutex, NULL);
//	pthread_cond_init(&condition_cond, NULL);
}

Assistant::~Assistant() {

}

std::pair<int,int> Assistant::replaceDishes(pair<int,int> collectedDishes)
{
	for (int j = 0; j< collectedDishes.first ; j++)
	{
		sem_post(&glasses);
		usleep(TIME_REPLACE_DISH *1000);
	}

	for (int j = 0; j< collectedDishes.second ; j++)
	{
		sem_post(&cups);
		usleep(TIME_REPLACE_DISH *1000);
	}

	return collectedDishes;
}

void Assistant::takeBreak()
{
	usleep(TIME_REST *1000);
}

std::pair<int,int> Assistant::collectDishes()
{
	int totalCollected = 0;
	pair<int,int> collectedDishes = pair<int,int>(0,0);

	for (int i = 0; i < NUM_TABLES; i++)
	{
		int this_tbl_units=0;
		DishType collectedDish;
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
		string msg = temp.str();
		log(name, msg);
//		assert(this_tbl_units <= NUM_TABLE_UNITS); it is possible, dishes are picked up one by one, it is possible to collect > 10

	}
	assert(totalCollected == collectedDishes.first + collectedDishes.second);
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

void Assistant::doCleanupRound()
{
	string s("starting cleanup");
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
	string msg = temp.str();
	log(name, msg);
}

//void* Assistant:: run(Landlord* landllord)
//void Assistant::run(Landlord* landllord)
//{
//		pthread_t assistantThread;
//		pthread_create(&assistantThread, NULL, Assistant::threadFun, (void *)landllord);
//}

void* Assistant::run_thread(void * landllord)
{
	Assistant* ass = new Assistant();

	//((Landlord*)landllord)->registerAssistent(this);
	ass->run();
	return 0;
}

void Assistant::run()
{
	ostringstream temp;
	temp << "Assistant got to work.";
	string msg = temp.str();
	log(name, msg);

	//while(!timeToFinish)
	while(!bClose)
	{
		takeBreak();
		doCleanupRound();
	}

	//last time!
	takeBreak();
	doCleanupRound();

	msg = "Clocking out.";
	log(name, msg);
	pthread_exit(NULL);
}

//I do not have any better idea how to solve final run in other way than this:
void Assistant::doFinalRun()
{
	timeToFinish = true;
}
