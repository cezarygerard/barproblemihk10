/*
 * BarEmp.h
 *
 *  Created on: Dec 12, 2010
 *      Author: jonathan
 */

#ifndef BAREMP_H_
#define BAREMP_H_

#include "Person.h"
#include "Customer.h"

class BarEmp: public Person {

protected:
	///message queue id to greet/say goodbye to landlord
	int drink_q_id;

public:
	///serve next Customer in drink queue
	void bartend();

	//this is used as argument for sending a drink order to drink message queue
	typedef struct __Drink_Msg_Args {
			///id of the customer
			long cust_id;
			///type of drink they ordered
			OrderType order_type;
			///pointer to the Customer's self
			Customer* cust_ptr;
		} Drink_Msg_Args;
};

#endif /* BAREMP_H_ */
