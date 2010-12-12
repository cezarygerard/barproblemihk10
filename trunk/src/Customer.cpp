/*
 * Customer.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Customer.h"
#include <sys/time.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "Table.h"

Customer::Customer() {
	ostringstream temp;
	temp << "Customer_" << currentCustID;
	name = temp.str();

	//sprintf(temp, "Customer_%s", currentCustID);
	//name = "Customer " + currentCustID;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);
	orderType = chooseDrink();
	drinksLeft = chooseMaxDrinks();
	favTableIndex = chooseFavTable();
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer::run(void* landllord)
{
	//char msg[256];
	//msg = "Entered the bar. TYPE:" /*+  typeAsString(orderType)*/ + "  DRINKS: " + maxDrinks + "  TABLE: " + favTableIndex;
	//sprintf(msg, "Entered the bar. TYPE: %s  DRINKS: %d  TABLE: %d", typeAsString(orderType), maxDrinks, favTableIndex);

	//Customer* c = new Customer();
	//((Landlord *)landllord)->registerAssistent(this)
	pthread_t customerThread;
	pthread_create(&customerThread, NULL, Customer::threadFun, (void *)landllord);
}

void* Customer::threadFun(void * landllord)
{
	Customer* c = new Customer();
	//((Landlord*)landllord)->registerAssistent(c);
	c->start();
	delete c;
	return NULL;
}

void Customer::start()
{
	ostringstream temp;
	temp << "Entered the bar. TYPE: " << typeAsString(orderType) << "  DRINKS: " << drinksLeft << "  TABLE: " << favTableIndex;
	string msg = temp.str();
	log(name, msg);

	while(drinksLeft > 0)
	{
		orderDrink();
		{
			ostringstream temp;
			temp <<"drink ordered";
			msg = temp.str();
			log(name, msg);
		}
		drink();
		drinksLeft--;
		{
			ostringstream temp;
			temp <<"drunk, drinks left: " << drinksLeft;
			msg = temp.str();
			log(name, msg);
		}


//		usleep(TIME_TO_DRINK * 1000);
	}


	//((Landlord*)landllord)->sayGoodBye(this);
	{
		ostringstream temp;
		temp.clear();
		temp <<"leaving...";
		msg = temp.str();
		log(name, msg);
	}
}

OrderType Customer::chooseDrink()
{
	//random number between 0 and 99
	int num = getRand() % 100;
	if (num < RATIO_BEER * 100)
		return BEER;
	else if (RATIO_BEER * 100 <= num && num < (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
		return CAPPUCCINO;
	//we know that the remaining proportion is due to RATIO_HOT_CHOCOLATE from initial assert()
	//else if (num >= (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
	//	return HOT_CHOCOLATE;
	//otherwise IDE said: "control reaches end of non-void function" [without return]
	else
		return HOT_CHOCOLATE;
	return BEER;
}

int Customer::chooseMaxDrinks()
{
	return getRand() % MAX_DRINKS_PER_CUST + 1;
}

int Customer::chooseFavTable()
{
	return getRand() % NUM_TABLES;
}

void Customer::orderDrink()
{

}

void Customer::drink()
{
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;

	if (orderType == BEER)
	{
		pthread_mutex_lock( &mutex );
		ts.tv_nsec = (TIME_TO_DRINK * msToNs + ts.tv_nsec) % nsToS;
		ts.tv_sec += (TIME_TO_DRINK * msToNs + ts.tv_nsec) / nsToS;
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
		pthread_cond_timedwait(&condition, &mutex, &ts);
		pthread_mutex_unlock( &mutex );
	}
	else
	{
		usleep(TIME_TO_DRINK * 1000);
	}

	DishType dt;
	switch (orderType) {
		case BEER:
			dt = GLASS;
			break;
		case CAPPUCCINO:
			dt = CUP;
			break;
		case HOT_CHOCOLATE:
			dt = CUP;
			break;
		default:
			break;
	}

	tables[favTableIndex].putDish(dt);
}

void Customer::lastOrder()
{
	pthread_cond_signal(&condition);
}
