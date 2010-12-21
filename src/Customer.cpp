/*
 * Customer.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Customer.h"
#include "Landlord.h"
#include "Table.h"

Customer::Customer(int cid, int dqi, int gqi) {
	ostringstream temp;
	temp << "Customer_" << cid;
	name = temp.str();

	//mutex for condition variables
	pthread_mutex_init(&mutex, NULL);
	//last call condition
	pthread_cond_init(&lc_condition, NULL);
	//drink order is ready condition
	pthread_cond_init(&rdy_condition, NULL);

	orderType = chooseDrink();
	drinksLeft = chooseMaxDrinks();
	favTableIndex = chooseFavTable();

	my_id = cid;
	drink_q_id = dqi;
	greeting_q_id = gqi;
}

Customer::~Customer() {

}

void* Customer::run_thread(void *dptr)
{
	Cust_Thread_Args *cptr = (Cust_Thread_Args *) dptr;
	Customer *cust = new Customer(cptr->cust_id, cptr->drink_q_id, cptr->greeting_q_id);
	cust->run();
	delete cust;
	return 0;
}

void Customer::run()
{
	ostringstream temp;
	temp << "Entered the bar. TYPE: " << typeAsString(orderType) << "  DRINKS: " << drinksLeft << "  TABLE: " << favTableIndex;
	log(name, temp.str());
	greetLandlord(false);

	//order a drink and then drink it until no drinks left
	while(drinksLeft > 0)
	{
		orderDrink();
		drink();
	}

	greetLandlord(true);
	pthread_exit(NULL);
}


void Customer::greetLandlord(bool leaving){
	Landlord::Greeting_Msg_Args tosend;
	tosend.leaving = leaving;
	tosend.person_id = my_id;
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
		msg = "Hello!";
	else
		msg = "See ya.";
	log(name, msg);
}

void Customer::lastOrder()
{
	//only beer drinkers are allowed one more drink
	pthread_mutex_lock( &mutex );
	//drinksLeft is going to get decremeted after he finishes, so set at 2.
	if (orderType == BEER && drinksLeft > 2)
		drinksLeft = 2;
	else
		drinksLeft = 1;
	//wake up all the beer drinkers so they can order one more if they want
	pthread_cond_signal(&lc_condition);
	pthread_mutex_unlock( &mutex );
}


void Customer::orderDrink()
{
	BarEmp::Drink_Msg_Args tosend;
	tosend.cust_id = my_id;
	tosend.order_type = orderType;
	tosend.cust_ptr = this;

	ostringstream temp;
	string msg;
	temp << "Waiting for a " << typeAsString(orderType) << "...";
	log(name, temp.str());
	int val = msgsnd(drink_q_id, (void*)&tosend, sizeof(BarEmp::Drink_Msg_Args), 0);
	if(val == -1) {
		msg = "ERROR: Failed to send drink request";
		log(name, msg);
		if(errno == EACCES)
		{
			msg = "ERROR: Please run program with elevated permissions.";
			log(msg);
			exit(0);
		}
	}
	else
	{
		pthread_mutex_lock( &mutex );
		pthread_cond_wait(&rdy_condition, &mutex);
		pthread_mutex_unlock( &mutex );

		temp.str("");
		temp << "Received " << typeAsString(orderType);
		msg = temp.str();
		log(name, msg);
	}
}

void Customer::receiveDrink()
{
	//tell the customer to wake up and "take drink"
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&rdy_condition);
	pthread_mutex_unlock( &mutex );
}

void Customer::drink()
{
	ostringstream temp;
	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;

	//wait the thread if drink beer
	if (orderType == BEER)
	{
		pthread_mutex_lock( &mutex );
		ts.tv_nsec = (TIME_TO_DRINK * msToNs + ts.tv_nsec) % nsToS;
		ts.tv_sec += (TIME_TO_DRINK * msToNs + ts.tv_nsec) / nsToS;
		pthread_cond_timedwait(&lc_condition, &mutex, &ts);
		drinksLeft--;
		pthread_mutex_unlock( &mutex );
	}
	//else, just sleep it
	else
	{
		pthread_mutex_lock( &mutex );
		usleep(TIME_TO_DRINK * 1000);
		drinksLeft--;
		pthread_mutex_unlock( &mutex );
	}

	temp <<"Finished! Drinks left: " << drinksLeft;
	log(name, temp.str());
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

	//put the dish on customer's table
	tables[favTableIndex].putDish(dt);
}

int Customer::chooseMaxDrinks()
{
	return getRand() % MAX_DRINKS_PER_CUST + 1;
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
	return HOT_CHOCOLATE;
}

int Customer::chooseFavTable()
{
	return getRand() % NUM_TABLES;
}

