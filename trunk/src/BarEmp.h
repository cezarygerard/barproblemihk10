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
	int drink_q_id;
public:
	void bartend();
	typedef struct __Drink_Msg_Args {
			long cust_id;
			OrderType order_type;
			Customer* cust_ptr;
		} Drink_Msg_Args;
};

#endif /* BAREMP_H_ */
