/*
 * Assistant.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Assistant.h"
#include "Common.h"
#include <iostream>
#include <sstream>
#include "Table.h"


Assistant::Assistant(){
	name = "Assistant";
	timeToFinish = false;
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init(&condition_cond, NULL);
}

Assistant::~Assistant() {

}

std::pair<int,int> Assistant::replaceDishes(pair<int,int> collectedDishes)
{
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;

	pthread_mutex_lock( &count_mutex );
	for (int j = 0; j< collectedDishes.first ; j++)
	{
		sem_post(&glasses);
		ts.tv_nsec = (TIME_REPLACE_DISH * msToNs + ts.tv_nsec) % nsToS;
		ts.tv_sec += (TIME_REPLACE_DISH* msToNs + ts.tv_nsec) / nsToS;
		pthread_cond_timedwait(&condition_cond, &count_mutex, &ts);
	}

	for (int j = 0; j< collectedDishes.second ; j++)
	{
		sem_post(&cups);
		ts.tv_nsec = (TIME_REPLACE_DISH * msToNs + ts.tv_nsec) % nsToS;
		ts.tv_sec += (TIME_REPLACE_DISH* msToNs + ts.tv_nsec) / nsToS;
		pthread_cond_timedwait(&condition_cond, &count_mutex, &ts);
	}
	pthread_mutex_unlock( &count_mutex );
	return collectedDishes;
}

void Assistant::takeBreak()
{
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;

	pthread_mutex_lock( &count_mutex );
	ts.tv_nsec = (TIME_REST * msToNs + ts.tv_nsec) % nsToS;
	ts.tv_sec += (TIME_REST * msToNs + ts.tv_nsec) / nsToS;
	/*TEST
	timeval now;
	gettimeofday(&now, NULL);
	cout<<"now:    " <<(long)now.tv_sec << " " <<(long) now.tv_usec<<endl;
	cout<<"wakeup: "
			<< (long)ts.tv_sec << " "
			<< (long)ts.tv_nsec
			<<endl;
	cout<<(TIME_REST * msToNs + ts.tv_nsec) % nsToS<<endl;
	cout<<(TIME_REST * msToNs + ts.tv_nsec) / nsToS<<endl;
	*/
	pthread_cond_timedwait(&condition_cond, &count_mutex, &ts);
	pthread_mutex_unlock( &count_mutex );
}

std::pair<int,int> Assistant::collectDishes()
{
	int totalCollected = 0;
	pair<int,int> collectedDishes = pair<int,int>(0,0);
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;
	pthread_mutex_lock( &count_mutex );
	for (int i = 0; i < NUM_TABLES; i++)
	{
		DishType collectedDish;
		while(!(tables[i].collectNextDish(collectedDish)))
		{
			totalCollected++;
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
			ts.tv_nsec = (TIME_COLLECT_DISH * msToNs + ts.tv_nsec) % nsToS;
			ts.tv_sec += (TIME_COLLECT_DISH * msToNs + ts.tv_nsec) / nsToS;
			pthread_cond_timedwait(&condition_cond, &count_mutex, &ts);
		}
		assert(totalCollected == collectedDishes.first + collectedDishes.second);
		/*TEST:
		timeval now;
		gettimeofday(&now, NULL);
		cout<< "err: "<< err << " current, : tableEmptyUnits "<< tableEmptyUnits << " tableDishesNum"<< tableDishesNum << "  "<<(long)now.tv_sec << " " <<(long) now.tv_usec<<endl;
		cout<<(long)ts.tv_sec << " " <<(long) ts.tv_nsec<<endl;
		 */
	}
	pthread_mutex_unlock( &count_mutex );
	return collectedDishes;
}

int Assistant::cleanDishes(int dishes)
{
	int cleanedDishes = 0; //artificial value, should always equal "dishes"
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;

	pthread_mutex_lock( &count_mutex );
	for (int j = 0; j< dishes ; j++)
	{
		ts.tv_nsec = (TIME_WASH_DISH * msToNs + ts.tv_nsec) % nsToS;
		ts.tv_sec += (TIME_WASH_DISH* msToNs + ts.tv_nsec) / nsToS;
		pthread_cond_timedwait(&condition_cond, &count_mutex, &ts);
		cleanedDishes++;
	}
	pthread_mutex_unlock( &count_mutex );
	return cleanedDishes;

}

void Assistant::doCleanupRound()
{
	takeBreak();
	pair<int,int> collected = collectDishes();
	int cleaned = cleanDishes(collected.first+collected.second);
	pair<int,int> replaced = replaceDishes(collected);

	//int returned = re
	ostringstream temp;
	temp << " glasses collected: " << collected.first
			<< " cups collected: " << collected.second
			<< "dishes cleaned: " <<cleaned
			<< " glasses replaced: " << replaced.first
			<< " cups replaced: " << replaced.first
			<<endl;
	string msg = temp.str();
	log(name, msg);
}
//void* Assistant:: run(Landlord* landllord)
void* Assistant::run(void* landllord)
{
	Assistant* ass = new Assistant();
	//((Landlord)landllord)->registerAssistent(this)
	ass->doStuff();
	return NULL;
}

void Assistant::doStuff()
{
	while(!timeToFinish)
	{
		doCleanupRound();
	}
		doCleanupRound();
}

//I do not have any better idea how to solve final run in other way than this:
void Assistant::doFinalRun()
{
	timeToFinish = true;
}
