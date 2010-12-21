/*
 * BarEmp.cpp
 *
 *  Created on: Dec 12, 2010
 *      Author: jonathan
 */

#include "BarEmp.h"
#include "Customer.h"

void BarEmp::bartend()
{
	ostringstream temp;
	Drink_Msg_Args msg_buf;
	//get next drink order details from queue
	int len = msgrcv(drink_q_id, (void*) &msg_buf, sizeof(Drink_Msg_Args), 0, IPC_NOWAIT);

	//something went wrong
	if(len == -1)
	{
		if(errno == ENOMSG)
		{
			usleep(1000);
			return;
		}

		temp.str("");
		temp << "error bartending - " << errno << "  q_id: " << drink_q_id;
		log(name, temp.str());
		//let's wait for a little
		usleep(250000);
	}

	OrderType type = msg_buf.order_type;
	switch (type) {
	case BEER:
		//obtain glass
		sem_wait(&glasses);
		//obtain the tap
		pthread_mutex_lock(&beerTap);
		//fill glass
		usleep(TIME_FILL_GLASS * 1000);
		pthread_mutex_unlock(&beerTap);
		break;
	case HOT_CHOCOLATE:
		//obtain a cup
		sem_wait(&cups);
		//obtain milk
		pthread_mutex_lock(&milk);
		//obtain chocolate
		pthread_mutex_lock(&chocolate);
		//mix drink
		usleep(TIME_MIX_DRINK * 1000);
		pthread_mutex_unlock(&chocolate);
		pthread_mutex_unlock(&milk);
		break;
	case CAPPUCCINO:
		//obtain a cup
		sem_wait(&cups);
		//obtain milk
		pthread_mutex_lock(&milk);
		//obtain coffee
		pthread_mutex_lock(&coffee);
		//mix drink
		usleep(TIME_MIX_DRINK * 1000);
		pthread_mutex_unlock(&coffee);
		pthread_mutex_unlock(&milk);
		break;
	}

	Customer* cust = msg_buf.cust_ptr;

	temp.str("");
	temp << "Gives " << cust->getName() << " a " << typeAsString(type);
	log(name, temp.str());
	//tell cust their drink is ready
	cust->receiveDrink();
}
