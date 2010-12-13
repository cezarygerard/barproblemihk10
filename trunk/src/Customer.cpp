/*
 * Customer.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Customer.h"

Customer::Customer(int cid, int cmqi, int dqi, int gqi) {
	ostringstream temp;
	temp << "Customer_" << cid;
	name = temp.str();

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);

	orderType = chooseDrink();
	drinksLeft = chooseMaxDrinks();
	favTableIndex = chooseFavTable();

	my_q_id = cmqi;
	my_id = cid;
	drink_q_id = dqi;
	greeting_q_id = gqi;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer::greetLandlord(bool leaving){
	Landlord::Greeting_Msg_Args tosend;
	tosend.cust_id = my_id;
	tosend.q_id = my_q_id;
	tosend.leaving = leaving;

	string msg = "";
	if(leaving)
		msg = "waiting to say goodbye to landlord";
	else
		msg = "Waiting for greeting from landlord.";
	log(name, msg);
	int val = msgsnd(greeting_q_id, (void*)&tosend, sizeof(Landlord::Greeting_Msg_Args), 0);
	if(val == -1) {
		msg = "failed to send greeting message";
		log(name, msg);
		if(errno == EACCES)
		{
			msg = "please run program with elevated permissions.";
			log(msg);
			exit(0);
		}
	}
	else
		msg = "Greeted.";
	log(name, msg);
}

void Customer::run()
{
	ostringstream temp;
	temp << "Entered the bar. TYPE: " << typeAsString(orderType) << "  DRINKS: " << drinksLeft << "  TABLE: " << favTableIndex;
	string msg = temp.str();
	log(name, msg);
	greetLandlord(false);

	//chillAtPub();

	usleep(1000000);
	greetLandlord(true);
}

void Customer::chillAtPub()
{
	string msg;
	ostringstream temp;
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


void* Customer::run_thread(void *dptr)
{
	Cust_Thread_Args *cptr = (Cust_Thread_Args *) dptr;
	Customer *cust = new Customer(cptr->cust_id, cptr->cust_msg_q_id, cptr->drink_q_id, cptr->greeting_q_id);
	cust->run();
	delete cust;
	return 0;
}
