/*
 * Customer.cpp
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#include "Customer.h"

Customer::Customer() {
	ostringstream temp;
	temp << "Customer_" << currentCustID;
	name = temp.str();

	//sprintf(temp, "Customer_%s", currentCustID);
	//name = "Customer " + currentCustID;

	orderType = chooseDrink();
	maxDrinks = chooseMaxDrinks();
	favTableIndex = chooseFavTable();
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer::run()
{
	//char msg[256];
	//msg = "Entered the bar. TYPE:" /*+  typeAsString(orderType)*/ + "  DRINKS: " + maxDrinks + "  TABLE: " + favTableIndex;
	//sprintf(msg, "Entered the bar. TYPE: %s  DRINKS: %d  TABLE: %d", typeAsString(orderType), maxDrinks, favTableIndex);

	ostringstream temp;
	temp << "Entered the bar. TYPE: " << typeAsString(orderType) << "  DRINKS: " << maxDrinks << "  TABLE: " << favTableIndex;
	string msg = temp.str();
	log(name, msg);

}

OrderType Customer::chooseDrink()
{
	//random number between 0 and 99
	int num = rand() % 100;
	if (num < RATIO_BEER * 100)
		return BEER;
	else if (RATIO_BEER * 100 <= num && num < (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
		return CAPPUCCINO;
	//we know that the remaining proportion is due to RATIO_HOT_CHOCOLATE from initial assert()
	else if (num >= (RATIO_CAPPUCCINO + RATIO_BEER) * 100)
		return HOT_CHOCOLATE;
	return BEER;
}

int Customer::chooseMaxDrinks()
{
	return rand() % MAX_DRINKS_PER_CUST + 1;
}

int Customer::chooseFavTable()
{
	return rand() % NUM_TABLES;
}
