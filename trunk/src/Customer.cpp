/*
 * Customer.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Customer.h"
#include "Common.h"

Customer::Customer() {
	name = "Customer " + currentCustID;
	srand(time(NULL));

	orderType = chooseDrink();
	maxDrinks = chooseMaxDrinks();
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer:: run()
{


}

OrderType chooseDrink()
{
	srand(time(NULL));
	//random number between 0 and 99
	int num = rand() % 100;
	if (num < RATIO_BEER * 100)
		return OrderType.beer;
	else if (RATIO_BEER * 100 <= num < (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
		return OrderType.cappuccino;
	//we know that the remaining proportion is due to RATIO_HOT_CHOCOLATE from initial assert()
	else if (num >= (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
		return OrderType.hot_chocolate;
}

int chooseMaxDrinks()
{
	srand(time(NULL));
	return rand() % MAX_DRINKS_PER_CUST + 1;
}

}
