/*
 * Customer.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Person.h"

class Customer: public Person {
public:
	Customer();
	virtual ~Customer();
	void run();
};

#endif /* CUSTOMER_H_ */
